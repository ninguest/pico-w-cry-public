#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "motor_control.h"

#define PWM_M1A 8
#define PWM_M1B 9
#define PWM_M2A 10
#define PWM_M2B 11

#define TCP_PORT 4242
#define DEBUG_printf printf
#define BUF_SIZE 2048
#define TEST_ITERATIONS 10
#define POLL_TIME_S 5

static struct tcp_pcb *tcp_server_pcb;
static char buffer[BUF_SIZE];

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

typedef struct CLIENT_T_ {
    struct tcp_pcb *client_pcb;
    struct CLIENT_T_ *next;
    uint8_t buffer_recv[BUF_SIZE];
    int recv_len;
} CLIENT_T;

typedef struct TCP_SERVER_T_ {
    struct tcp_pcb *server_pcb;
    CLIENT_T *clients;
    bool complete;
} TCP_SERVER_T;

static TCP_SERVER_T* tcp_server_init(void) {
    TCP_SERVER_T *state = calloc(1, sizeof(TCP_SERVER_T));
    if (!state) {
        DEBUG_printf("failed to allocate state\n");
        return NULL;
    }
    return state;
}

static void tcp_server_remove_client(TCP_SERVER_T *state, CLIENT_T *client) {
    CLIENT_T **p = &state->clients;
    while (*p && *p != client) {
        p = &(*p)->next;
    }
    if (*p) {
        *p = client->next;
        free(client);
    }
}

static err_t tcp_server_close(void *arg, CLIENT_T *client) {
    err_t err = ERR_OK;
    if (client->client_pcb != NULL) {
        tcp_arg(client->client_pcb, NULL);
        tcp_poll(client->client_pcb, NULL, 0);
        tcp_sent(client->client_pcb, NULL);
        tcp_recv(client->client_pcb, NULL);
        tcp_err(client->client_pcb, NULL);
        err = tcp_close(client->client_pcb);
        if (err != ERR_OK) {
            DEBUG_printf("close failed %d, calling abort\n", err);
            tcp_abort(client->client_pcb);
            err = ERR_ABRT;
        }
        client->client_pcb = NULL;
    }
    return err;
}

static err_t tcp_server_result(TCP_SERVER_T *state, CLIENT_T *client, int status) {
    if (status == 0) {
        DEBUG_printf("test success\n");
    } else {
        DEBUG_printf("test failed %d\n", status);
    }
    tcp_server_close(state, client);
    tcp_server_remove_client(state, client);
    return ERR_OK;
}

static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    CLIENT_T *client = (CLIENT_T*)arg;
    DEBUG_printf("tcp_server_sent %u\n", len);
    return ERR_OK;
}

static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    CLIENT_T *client = (CLIENT_T*)arg;
    TCP_SERVER_T *state = (TCP_SERVER_T*)client->client_pcb->callback_arg;
    if (!p) {
        DEBUG_printf("Client closed the connection\n");
        tcp_server_result(state, client, 0);
        return ERR_OK;
    }
    cyw43_arch_lwip_check();
    if (p->tot_len > 0) {
        DEBUG_printf("tcp_server_recv %d/%d err %d\n", p->tot_len, client->recv_len, err);

        // Print the received message
        char *received_data = (char *)malloc(p->tot_len + 1);
        if (received_data) {
            memcpy(received_data, p->payload, p->tot_len);
            received_data[p->tot_len] = '\0'; // Null-terminate the string
            DEBUG_printf("Received message: %s\n", received_data);
            free(received_data);
        }

        // Echo the received data back to the client
        err_t err = tcp_write(tpcb, p->payload, p->tot_len, TCP_WRITE_FLAG_COPY);
        if (err != ERR_OK) {
            DEBUG_printf("Failed to write data %d\n", err);
            pbuf_free(p);
            return tcp_server_result(state, client, -1); // Close the connection on write error
        }
        tcp_recved(tpcb, p->tot_len);
    }
    pbuf_free(p);

    return ERR_OK; // Keep the connection open
}

static err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb) {
    DEBUG_printf("tcp_server_poll_fn\n");
    return ERR_OK;
}

static void tcp_server_err(void *arg, err_t err) {
    if (err != ERR_ABRT) {
        DEBUG_printf("tcp_client_err_fn %d\n", err);
        CLIENT_T *client = (CLIENT_T*)arg;
        TCP_SERVER_T *state = (TCP_SERVER_T*)client->client_pcb->callback_arg;
        tcp_server_result(state, client, err);
    }
}

static err_t tcp_server_accept(void *arg, struct tcp_pcb *client_pcb, err_t err) {
    TCP_SERVER_T *state = (TCP_SERVER_T*)arg;
    if (err != ERR_OK || client_pcb == NULL) {
        DEBUG_printf("Failure in accept\n");
        return ERR_VAL;
    }
    DEBUG_printf("Client connected\n");

    CLIENT_T *client = calloc(1, sizeof(CLIENT_T));
    if (!client) {
        DEBUG_printf("Failed to allocate client\n");
        return ERR_MEM;
    }
    client->client_pcb = client_pcb;
    client->next = state->clients;
    state->clients = client;

    client_pcb->callback_arg = state;
    tcp_arg(client_pcb, client);
    tcp_sent(client_pcb, tcp_server_sent);
    tcp_recv(client_pcb, tcp_server_recv);
    tcp_poll(client_pcb, tcp_server_poll, POLL_TIME_S * 2);
    tcp_err(client_pcb, tcp_server_err);

    return ERR_OK;
}

static bool tcp_server_open(void *arg) {
    TCP_SERVER_T *state = (TCP_SERVER_T*)arg;
    DEBUG_printf("Starting server at %s on port %u\n", ip4addr_ntoa(netif_ip4_addr(netif_list)), TCP_PORT);

    struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
    if (!pcb) {
        DEBUG_printf("failed to create pcb\n");
        return false;
    }

    err_t err = tcp_bind(pcb, NULL, TCP_PORT);
    if (err) {
        DEBUG_printf("failed to bind to port %u\n", TCP_PORT);
        return false;
    }

    state->server_pcb = tcp_listen_with_backlog(pcb, 1);
    if (!state->server_pcb) {
        DEBUG_printf("failed to listen\n");
        if (pcb) {
            tcp_close(pcb);
        }
        return false;
    }

    tcp_arg(state->server_pcb, state);
    tcp_accept(state->server_pcb, tcp_server_accept);

    return true;
}

void run_tcp_server_test(void) {
    TCP_SERVER_T *state = tcp_server_init();
    if (!state) {
        return;
    }
    if (!tcp_server_open(state)) {
        free(state);
        return;
    }
    while(!state->complete) {
#if PICO_CYW43_ARCH_POLL
        cyw43_arch_poll();
        cyw43_arch_wait_for_work_until(make_timeout_time_ms(1000));
#else
        sleep_ms(1000);
#endif
    }
    free(state);
}

// WIFI Credentials - take care if pushing to github!
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
    printf("Connecting to Wi-Fi...\n");
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
    
    // Run TCP server test
    run_tcp_server_test();

    // Infinite loop
    while(1){
        // Print the IP address
        struct netif *netif = netif_list;
        if (netif != NULL) {
            printf("IP Address: %s\n", ip4addr_ntoa(&netif->ip_addr));
        } else {
            printf("No network interface found.\n");
        }
        sleep_ms(5000); // Sleep for 5 seconds
    };
}