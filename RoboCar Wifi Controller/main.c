#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"

#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "motor_control.h"

#define PWM_M1A 8
#define PWM_M1B 9
#define PWM_M2A 10
#define PWM_M2B 11

void setup_motors() {
    gpio_set_function(PWM_M1A, GPIO_FUNC_PWM);
    gpio_set_function(PWM_M1B, GPIO_FUNC_PWM);
    gpio_set_function(PWM_M2A, GPIO_FUNC_PWM);
    gpio_set_function(PWM_M2B, GPIO_FUNC_PWM);

    pwm_set_wrap(pwm_gpio_to_slice_num(PWM_M1A), 10000);
    pwm_set_wrap(pwm_gpio_to_slice_num(PWM_M1B), 10000);
    pwm_set_wrap(pwm_gpio_to_slice_num(PWM_M2A), 10000);
    pwm_set_wrap(pwm_gpio_to_slice_num(PWM_M2B), 10000);

    pwm_set_enabled(pwm_gpio_to_slice_num(PWM_M1A), true);
    pwm_set_enabled(pwm_gpio_to_slice_num(PWM_M1B), true);
    pwm_set_enabled(pwm_gpio_to_slice_num(PWM_M2A), true);
    pwm_set_enabled(pwm_gpio_to_slice_num(PWM_M2B), true);
}

void set_motor_speed(uint slice_num, float speed) {
    uint16_t level = (uint16_t)(speed * 10000);
    pwm_set_gpio_level(slice_num, level);
}

void move_forward() {
    set_motor_speed(PWM_M1A, 0.5);
    set_motor_speed(PWM_M1B, 0.0);
    set_motor_speed(PWM_M2A, 0.5);
    set_motor_speed(PWM_M2B, 0.0);
}

void move_backward() {
    set_motor_speed(PWM_M1A, 0.0);
    set_motor_speed(PWM_M1B, 0.5);
    set_motor_speed(PWM_M2A, 0.0);
    set_motor_speed(PWM_M2B, 0.5);
}

void move_left() {
    set_motor_speed(PWM_M1A, 0.0);
    set_motor_speed(PWM_M1B, 0.5);
    set_motor_speed(PWM_M2A, 0.5);
    set_motor_speed(PWM_M2B, 0.0);
}

void move_right() {
    set_motor_speed(PWM_M1A, 0.5);
    set_motor_speed(PWM_M1B, 0.0);
    set_motor_speed(PWM_M2A, 0.0);
    set_motor_speed(PWM_M2B, 0.5);
}

void move_stop() {
    set_motor_speed(PWM_M1A, 0.0);
    set_motor_speed(PWM_M1B, 0.0);
    set_motor_speed(PWM_M2A, 0.0);
    set_motor_speed(PWM_M2B, 0.0);
}

// WIFI Credentials 
const char WIFI_SSID[] = "SSID";
const char WIFI_PASSWORD[] = "PASSWORD";

int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("Failed to initialize cyw43_arch\n");
        return -1;
    }

    // Enable station mode
    cyw43_arch_enable_sta_mode();

    // Connect to WiFi
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Failed to connect to WiFi\n");
        return -1;
    }

    // Print a success message once connected
    printf("Connected! \n");

    // Initialise web server
    httpd_init();
    printf("Http server initialised\n");

    // Configure SSI and CGI handler
    ssi_init(); 
    printf("SSI Handler initialised\n");
    cgi_init();
    printf("CGI Handler initialised\n");

    // Initialise motor control
    setup_motors();
    
    // Infinite loop
    while(1){
        // Print the IP address
        struct netif *netif = netif_list;
        printf("IP Address: %s\n", ip4addr_ntoa(&netif->ip_addr));
        sleep_ms(5000); // Sleep for 1 second
    };
}