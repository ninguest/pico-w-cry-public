#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"

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
    
    // Infinite loop
    while(1){
        // Print the IP address
        struct netif *netif = netif_list;
        printf("IP Address: %s\n", ip4addr_ntoa(&netif->ip_addr));
        sleep_ms(5000); // Sleep for 1 second
    };
}