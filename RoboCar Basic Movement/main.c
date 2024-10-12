#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdint.h>
#include <stdbool.h>  // Include this header for true/false definitions

// Define GPIO pins for motors
#define PWM_M1A 8
#define PWM_M1B 9
#define PWM_M2A 10
#define PWM_M2B 11

// Function to set PWM duty cycle
void set_pwm_duty_cycle(uint32_t gpio, float duty_cycle) {
    uint32_t slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_gpio_level(gpio, duty_cycle * 65535);
}

// Function to initialize PWM
void init_pwm(uint32_t gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint32_t slice_num = pwm_gpio_to_slice_num(gpio);
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

    // Test robot movement
    while (true) {
        Robot_Movement(0, 0);  // Stop
        sleep_ms(2000);
        Robot_Movement(0.5, 0.54);  // Forward
        sleep_ms(3000);
        Robot_Movement(-0.5, -0.52);  // Backward
        sleep_ms(3000);
        Robot_Movement(0.1, 0.5);  // Turn Left
        sleep_ms(3000);
        Robot_Movement(0.5, 0.1);  // Turn Right
        sleep_ms(3000);
    }

    return 0;
}