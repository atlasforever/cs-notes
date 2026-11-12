cmake_minimum_required(VERSION 3.16)
project(freertos_posix_lab LANGUAGES C)

set(FREERTOS_PORT GCC_POSIX CACHE STRING "")
set(FREERTOS_HEAP 4 CACHE STRING "")

add_library(freertos_config INTERFACE)
target_include_directories(freertos_config INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(third_party/FreeRTOS-Kernel)

find_package(Threads REQUIRED)

add_executable(posix_demo main.c)

target_link_libraries(posix_demo PRIVATE
    freertos_kernel
    Threads::Threads
)