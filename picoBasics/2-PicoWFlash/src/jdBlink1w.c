/**
 * Jon Durrant.
 *
 * Blink LED on Raspberry PICO W
 */

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#define DELAY 500 // in microseconds

int main() {
	stdio_init_all();
	cyw43_arch_init();

    while (true) { // Loop forever
    	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
		printf("LED is ON\n");
		sleep_ms(DELAY);
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
		printf("LED is OFF\n");
		sleep_ms(DELAY);
    }

}
