set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m3)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)

# 裸机环境暂时没有完整的可执行文件链接配置。
# CMake 的编译器测试采用静态库方式，避免尝试链接可执行文件。
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)