add_executable(${ProjectName}
    main.c
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${ProjectName}
    pico_cyw43_arch_lwip_threadsafe_background
    pico_stdlib
    pico_lwip
    pico_lwip_http
    pico_lwip_iperf
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
    hardware_adc
    hardware_pwm
)

pico_enable_stdio_usb(${ProjectName} 1)
pico_enable_stdio_uart(${ProjectName} 0)

pico_add_extra_outputs(${ProjectName})



