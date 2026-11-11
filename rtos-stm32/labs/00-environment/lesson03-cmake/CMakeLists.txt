cmake_minimum_required(VERSION 3.16)

project(cmake_cross_lab LANGUAGES C)

add_library(combine OBJECT main.c)

set_target_properties(combine PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

target_compile_options(combine PRIVATE
    -Wall
    -Wextra
)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-m3")
    target_compile_options(combine PRIVATE
        -mcpu=cortex-m3
        -mthumb
        -ffreestanding
    )
endif()