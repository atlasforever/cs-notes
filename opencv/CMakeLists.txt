cmake_minimum_required(VERSION 3.16)

project(opencv_samples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCV REQUIRED)

file(GLOB SAMPLE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(sample_source IN LISTS SAMPLE_SOURCES)
    get_filename_component(sample_name "${sample_source}" NAME_WE)

    add_executable("${sample_name}" "${sample_source}")
    target_link_libraries("${sample_name}" PRIVATE ${OpenCV_LIBS})
    target_include_directories("${sample_name}" PRIVATE ${OpenCV_INCLUDE_DIRS})
endforeach()
