cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(robot_movement)

pico_sdk_init()

add_executable(robot_movement main.c)

target_link_libraries(robot_movement pico_stdlib hardware_pwm)

pico_add_extra_outputs(robot_movement)