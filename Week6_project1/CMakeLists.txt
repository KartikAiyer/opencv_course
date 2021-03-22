cmake_minimum_required(VERSION 3.11)
project(project1 CXX)

set(data_path "${CMAKE_CURRENT_SOURCE_DIR}/data")

add_library(project1_interface INTERFACE )
target_compile_definitions(project1_interface INTERFACE -DDATA_PATH="${data_path}")

add_subdirectory(instagram_filters)
add_subdirectory(blemish_removal)