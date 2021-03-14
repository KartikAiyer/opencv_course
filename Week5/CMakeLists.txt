cmake_minimum_required(VERSION 3.11)

project(Week5 CXX)
set(images_path ${CMAKE_CURRENT_SOURCE_DIR}/week5_cpp/data/images)
add_library(week5_interface INTERFACE)
target_compile_definitions(week5_interface INTERFACE -DIMAGES_PATH="${images_path}")

add_subdirectory(week5_cpp)