cmake_minimum_required(VERSION 3.11)
project(Week10 CXX)

set(data_path "${CMAKE_CURRENT_SOURCE_DIR}/week10-cpp/data")

add_library(week10_interface INTERFACE)
target_compile_definitions(week10_interface INTERFACE -DDATA_FOLDER="${data_path}")

add_subdirectory(week10-cpp)

