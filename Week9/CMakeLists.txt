cmake_minimum_required(VERSION 3.11)
project(Week9 CXX)

set(data_path "${CMAKE_CURRENT_SOURCE_DIR}/week9-cpp/data")

add_library(week9_interface INTERFACE)
target_compile_definitions(week9_interface INTERFACE -DDATA_FOLDER="${data_path}")

add_subdirectory(week9-cpp)