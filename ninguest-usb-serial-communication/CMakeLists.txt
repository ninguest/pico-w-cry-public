cmake_minimum_required(VERSION 3.13)

set(PROJECTNAME usb_serial_communication)
set(CMAKE C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD pico_w)

include(pico_sdk_import.cmake)

project(${PROJECTNAME})

pico_sdk_init()

# Add the directory containing lwipopts.h to the include path
include_directories(${PROJECTNAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_LIST_DIR}
)

add_executable(${PROJECTNAME}
    main.c
)

# Add pico_stdlib library, add more if used
target_link_libraries(${PROJECTNAME} 
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    hardware_adc
    hardware_gpio
    hardware_pwm
)

# enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECTNAME} 1)
pico_enable_stdio_uart(${PROJECTNAME} 0)

# Need to generate UF2 file for upload to RP2040
pico_add_extra_outputs(${PROJECTNAME})