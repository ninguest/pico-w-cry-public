#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include <stdbool.h>

// Define GPIO pins for motors
#define PWM_M1A 8
#define PWM_M1B 9
#define PWM_M2A 10
#define PWM_M2B 11

// Function to set PWM duty cycle
void set_pwm_duty_cycle(uint gpio, float duty_cycle) {
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_gpio_level(gpio, duty_cycle * 65535);
}

// Function to initialize PWM
void init_pwm(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_enabled(slice_num, true);
}

// Function to control robot movement
void Robot_Movement(float speed_left, float speed_right) {
    set_pwm_duty_cycle(PWM_M1A, speed_left > 0 ? speed_left : 0);
    set_pwm_duty_cycle(PWM_M1B, speed_left < 0 ? -speed_left : 0);
    set_pwm_duty_cycle(PWM_M2A, speed_right > 0 ? speed_right : 0);
    set_pwm_duty_cycle(PWM_M2B, speed_right < 0 ? -speed_right : 0);
}

int main() {
    // Initialize chosen serial port
    stdio_init_all();

    // Initialize PWM for motor control pins
    init_pwm(PWM_M1A);
    init_pwm(PWM_M1B);
    init_pwm(PWM_M2A);
    init_pwm(PWM_M2B);

    // Initialize ADC for sensor input
    adc_init();
    adc_gpio_init(26); // GP26 is ADC0
    adc_select_input(0);

    while (true) {
        // Read analog value from sensor
        uint16_t raw = adc_read();
        float an = (raw * 3.3) / 4096.0;
        printf("Analog value: %f\n", an);

        // Control robot movement based on sensor value
        if (an >= 1.4 && an <= 1.5) {  // 1.4 - 1.5v
            printf("Move forward\n");
            Robot_Movement(0.5, 0.53);
        } else if (an > 1.5 && an <= 2.2) { // 1.5 - 2.2v
            Robot_Movement(0.5, 0.3);
        } else if (an > 0.8 && an <= 1.4) { // 0.8 - 1.4v
            Robot_Movement(0.3, 0.53);
        } else if (an > 2.2 && an <= 2.85) { // 2.2 - 2.85v
            Robot_Movement(0.6, 0.2);
        } else if (an > 0.4 && an <= 0.8) { // 0.4 - 0.8v
            Robot_Movement(0.2, 0.63);
        } else if (an > 2.85 && an <= 3.0) { // 2.85 - 3.0v
            Robot_Movement(0.6, 0);
        } else if (an > 0.3 && an <= 0.4) { // 0.3 - 0.4v
            Robot_Movement(0, 0.64);
        } else if (an <= 0.3 || an > 3.0) { // <0.3v or >3V
            // Stop or continue as needed
            Robot_Movement(0, 0);
            sleep_ms(200);
            Robot_Movement(-0.3, -0.3);
            sleep_ms(200);
            Robot_Movement(0, 0);
        }

        sleep_ms(100); // Add a small delay
    }
    return 0;
}