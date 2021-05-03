cmake_minimum_required(VERSION 3.11)
project(Week8 CXX)

set(data_path "${CMAKE_CURRENT_SOURCE_DIR}/week8-cpp/data")

add_library(week8_interface INTERFACE)
target_compile_definitions(week8_interface INTERFACE -DDATA_FOLDER="${data_path}")

add_subdirectory(week8-cpp)