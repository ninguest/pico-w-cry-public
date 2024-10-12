cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(line_following_robot)

# Initialize the Pico SDK
pico_sdk_init()

# Add executable
add_executable(line_following_robot
    main.c
    header.h  # Include any additional headers if necessary
)

# Link libraries
target_link_libraries(line_following_robot
    pico_stdlib
    hardware_pwm
    hardware_adc
)

# Enable USB output, disable UART output
pico_enable_stdio_usb(line_following_robot 1)
pico_enable_stdio_uart(line_following_robot 0)

# Add extra outputs (uf2, bin, hex)
pico_add_extra_outputs(line_following_robot)