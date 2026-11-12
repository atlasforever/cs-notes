cmake_minimum_required(VERSION 3.16)
project(qemu_freertos LANGUAGES C ASM)

add_compile_options(
    -mcpu=cortex-m3
    -mthumb
    -ffreestanding
    -ffunction-sections
    -fdata-sections
    -Wall
    -Wextra
)

set(FREERTOS_PORT GCC_ARM_CM3 CACHE STRING "")
set(FREERTOS_HEAP 4 CACHE STRING "")

add_library(freertos_config INTERFACE)
target_include_directories(freertos_config INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(
    ../lesson04-posix/third_party/FreeRTOS-Kernel
    ${CMAKE_CURRENT_BINARY_DIR}/freertos-kernel
)

add_executable(firmware.elf
    startup.S
    main.c
    runtime.c
)

target_link_libraries(firmware.elf PRIVATE
    freertos_kernel
    gcc
)

target_link_options(firmware.elf PRIVATE
    -mcpu=cortex-m3
    -mthumb
    -nostdlib
    -T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
    -Wl,--gc-sections
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/firmware.map
)