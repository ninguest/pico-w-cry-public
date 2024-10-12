#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/cyw43_arch.h"

#define MAX_MESSAGE_SIZE 100

int main() {
    stdio_init_all();
    if (cyw43_arch_init() != 0) {
        printf("CYW43 initialization failed\n");
        return -1;
    }

    char userInput[MAX_MESSAGE_SIZE]; // Buffer to store user input
    int index; // Index to keep track of the current position in the buffer
    char ch; // Variable to store the current character read from the user

    while (1) {
        if (stdio_usb_connected()) {
            printf("USB connected\n");
            printf("Command (on/off): \n");

            // Read user input
            index = 0;
            ch = getchar(); // Get the first character

            // Loop until the user presses Enter
            while(1){
                if (ch == '\n' || ch == '\r') { // Check if the user pressed Enter
                    userInput[index] = '\0'; // Null-terminate the string
                    break;
                } else {
                    printf("%c", ch); // Echo the character back to the user
                    userInput[index] = ch;
                    index++;
                    ch = getchar(); // Get the next character
                }
            }

            // Process user input
            printf("\nYou entered: '%s'\n", userInput); // Debugging line to show the input
            
            if (strcmp(userInput, "on") == 0) {
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1); // Turn LED on
                printf("LED is ON\n");
            } else if (strcmp(userInput, "off") == 0) {
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0); // Turn LED off
                printf("LED is OFF\n");
            } else if (strcmp(userInput, "hello") == 0) {
                printf("Hello, World!\n");
            }
            else {
                printf("Invalid command. Please enter 'on' or 'off'.\n");
            }
        } else {
            printf("USB not connected\n");
        }
        sleep_ms(1000);
    }
    return 0;
}