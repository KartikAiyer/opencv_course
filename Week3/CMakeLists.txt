cmake_minimum_required(VERSION 3.11)

project(Week3 CXX)
set(images_path ${CMAKE_CURRENT_SOURCE_DIR}/week3-cpp/data)
add_library(week3_interface INTERFACE)
target_compile_definitions(week3_interface INTERFACE -DDATA_PATH="${images_path}")

add_subdirectory(ErosionDilation)
add_subdirectory(week3-cpp)
add_subdirectory(CoinDetection)