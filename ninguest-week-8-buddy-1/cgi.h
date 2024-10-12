#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "motor_control.h"

// CGI handler which is run when a request for /led.cgi is detected
const char * cgi_led_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    // Check if an request for LED has been made (/led.cgi?led=x)
    if (strcmp(pcParam[0] , "led") == 0){
        // Look at the argument to check if LED is to be turned on (x=1) or off (x=0)
        if(strcmp(pcValue[0], "0") == 0)
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        else if(strcmp(pcValue[0], "1") == 0)
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    }
    
    // Send the index page back to the user
    return "/index.shtml";
}

// CGI handler which is run when a request for /motor.cgi is detected
const char *cgi_motor_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    if (strcmp(pcParam[0], "action") == 0) {
        if (strcmp(pcValue[0], "forward") == 0) {
            move_forward();
        } else if (strcmp(pcValue[0], "backward") == 0) {
            move_backward();
        } else if (strcmp(pcValue[0], "left") == 0) {
            move_left();
        } else if (strcmp(pcValue[0], "right") == 0) {
            move_right();
        } else if (strcmp(pcValue[0], "stop") == 0) {
            move_stop();
        }
    }
    return "/index.shtml";
}

// tCGI Struct
// Fill this with all of the CGI requests and their respective handlers
static const tCGI cgi_handlers[] = {
    {
        // Html request for "/led.cgi" triggers cgi_handler
        "/led.cgi", cgi_led_handler
    },
    {
        // Html request for "/motor.cgi" triggers cgi_motor_handler
        "/motor.cgi", cgi_motor_handler
    },
};

void cgi_init(void)
{
    http_set_cgi_handlers(cgi_handlers, sizeof(cgi_handlers) / sizeof(tCGI));
}