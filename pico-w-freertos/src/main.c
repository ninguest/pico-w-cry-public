#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <queue.h>
#include "message_buffer.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"
#include "lwip/apps/httpd.h"
#include "lwipopts.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/icmp.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/inet.h"
#include "lwip/inet_chksum.h"

#define MESSAGE_BUFFER_SIZE 128 // Size of the message buffer
#define MOVING_AVERAGE_SIZE 10 // Size of the moving average

static QueueHandle_t xQueue = NULL; // Queue handle
static MessageBufferHandle_t xMessageBuffer = NULL; // Message buffer handle

const char WIFI_SSID[] = "SSID"; // WiFi SSID
const char WIFI_PASSWORD[] = "PASSWORD"; // WiFi password

// main_task : sends a ping to a given IP address
void main_task(void *pvParameters){
    const char *ip_addr_str = (const char *)pvParameters; // IP address to ping
    ip_addr_t target_ip;
    inet_aton(ip_addr_str, &target_ip);

    while (1) {
        struct icmp_echo_hdr *iecho; // ICMP header
        struct pbuf *p; // Packet buffer
        struct netif *netif = netif_list; // Network interface

        // Allocate a pbuf for the ICMP packet
        p = pbuf_alloc(PBUF_IP, sizeof(struct icmp_echo_hdr), PBUF_RAM);
        if (!p) {
            printf("FROM PING TASK: Failed to allocate pbuf\n");
            vTaskDelay(1000);
            continue;
        }
        
        // Fill in the ICMP header
        iecho = (struct icmp_echo_hdr *)p->payload;
        ICMPH_TYPE_SET(iecho, ICMP_ECHO);
        ICMPH_CODE_SET(iecho, 0);
        iecho->chksum = 0;
        iecho->seqno = htons(1);
        iecho->id = htons(1);
        iecho->chksum = inet_chksum(iecho, p->len);

        if (netif->output(netif, p, &target_ip) != ERR_OK) {
            printf("FROM PING TASK: Failed to send ping\n");
        } else {
            printf("FROM PING TASK: Ping sent to %s\n", ip_addr_str);
        }

        // Free the pbuf
        pbuf_free(p);
        vTaskDelay(1000);
    }
}

// led_task : blinks and LED at regular intervals
void led_task(void *pvParameters){
    uint uIValueToSend = 0;

    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        printf("FROM LED TASK: LED ON\n");
        // uIValueToSend = 1;
        // xQueueSend(xQueue, &uIValueToSend, 0U);
        vTaskDelay(1000);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        printf("FROM LED TASK: LED OFF\n");
        // uIValueToSend = 0;
        // xQueueSend(xQueue, &uIValueToSend, 0U);
        vTaskDelay(1000);
    }
}

// usb_task : prints the state of the LED
void usb_task(void *pvParameters){
    uint uIValueReceived = 0;

    while (1)
    {
        xQueueReceive(xQueue, &uIValueReceived, portMAX_DELAY);

        if(uIValueReceived == 1)
        {
            printf("FROM USB TASK: LED is ON\n");
        }
        if(uIValueReceived == 0)
        {
            printf("FROM USB TASK:LED is OFF\n");
        }
    }
}

// ip_task : prints the IP address of the device
void ip_task(void *pvParameters){
    while (1) {
        struct netif *netif = netif_list;
        printf("FROM IP TASK: My IP Address is %s\n", ip4addr_ntoa(netif_ip4_addr(netif)));
        vTaskDelay(5000); // Print IP address every 5 seconds
    }
}

// temp_task : reads temperature data and prints out and sends it to a message buffer
void temp_task(void *pvParameters){
    adc_init(); // Initialize the ADC
    adc_set_temp_sensor_enabled(true); // Enable the temperature sensor
    adc_select_input(4); // Select the temperature sensor input

    while(true){
        uint16_t raw = adc_read(); // Read the raw temperature data
        float voltage = raw * 3.3f / (1 << 12); // Convert the raw data to voltage
        float temperature = 27.0f - (voltage - 0.706f) / 0.001721f; // Convert the voltage to temperature
        printf("Temperature: %.2f\n", temperature); // Print the temperature

        // Send the temperature data to the message buffer
        xMessageBufferSend(xMessageBuffer, &temperature, sizeof(temperature), portMAX_DELAY);
        vTaskDelay(1000);
    }
}

// avg_task : ead temperature data from the message buffer and calculate and print the moving average.
void avg_task(void *pvParameters){
    float temperatures[MOVING_AVERAGE_SIZE] = {0}; // Array to store the last MOVING_AVERAGE_SIZE temperatures
    int index = 0; // Index of the next temperature to be added to the array
    int count = 0; // Number of temperatures in the array

    while (true) {
        float temperature;
        // Receive the temperature data from the message buffer
        xMessageBufferReceive(xMessageBuffer, &temperature, sizeof(temperature), portMAX_DELAY);

        temperatures[index] = temperature; // Add the temperature to the array
        index = (index + 1) % MOVING_AVERAGE_SIZE; // Update the index

        // Calculate the moving average
        if (count < MOVING_AVERAGE_SIZE) { 
            count++;
        }

        // Calculate the sum of the temperatures
        float sum = 0;

        // Calculate the sum of the temperatures
        for (int i = 0; i < count; i++) {
            sum += temperatures[i];
        }

        // Calculate the average temperature
        float average = sum / count;
        printf("Moving Average Temperature: %.2f C\n", average);
    }
}

// main : initializes the WiFi module, creates tasks, and starts the scheduler
int main()
{
    stdio_init_all(); // Initialize stdio

    // Initialize the CYW43 architecture
    if (cyw43_arch_init()) {
        printf("Failed to initialize cyw43_arch\n");
        return -1;
    }

    //Enable station mode
    cyw43_arch_enable_sta_mode();

    //Connect to the WiFi network
    if(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000))
    {
        printf("Failed to connect to the WiFi network\n");
        return -1;
    }

    // Print a success message once connected
    printf("Connected to the WiFi network\n");

    xQueue = xQueueCreate(1, sizeof(uint)); // Create a queue
    xMessageBuffer = xMessageBufferCreate(MESSAGE_BUFFER_SIZE); // Create a message buffer

    // Create tasks
    if (xQueue != NULL && xMessageBuffer != NULL) {
        xTaskCreate(main_task, "Main_Task", 256, "8.8.8.8", 1, NULL); // Ping task
        xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL); // LED task
        xTaskCreate(temp_task, "Temp_Task", 256, NULL, 1, NULL); // Temperature task
        xTaskCreate(avg_task, "Avg_Task", 256, NULL, 1, NULL); // Moving average task
        vTaskStartScheduler(); // Start the scheduler
    } else {
        printf("Failed to create queue\n");
    }

    while(1){};
}