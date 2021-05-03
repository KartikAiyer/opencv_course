cmake_minimum_required(VERSION 3.11)
project(Week7 CXX)

set(data_path "${CMAKE_CURRENT_SOURCE_DIR}/week7-cpp/data")

add_library(week7_interface INTERFACE)
target_compile_definitions(week7_interface INTERFACE -DDATA_FOLDER="${data_path}")

add_subdirectory(week7-cpp)
add_subdirectory(Panorama)