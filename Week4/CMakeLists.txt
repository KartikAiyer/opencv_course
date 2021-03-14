cmake_minimum_required(VERSION 3.11)

project(Week4 CXX)
set(images_path ${CMAKE_CURRENT_SOURCE_DIR}/week4-cpp/data/images)
add_library(week4_interface INTERFACE)
target_compile_definitions(week4_interface INTERFACE -DIMAGES_PATH="${images_path}")

add_subdirectory(week4-cpp)
add_subdirectory(ColorConversion)
add_subdirectory(Filtering)
add_subdirectory(Autofocus)