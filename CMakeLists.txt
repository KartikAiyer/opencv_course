cmake_minimum_required(VERSION 3.11)

project(opencv_course CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(OpenCV_DIR "${OpenCV_Install}/lib/cmake/opencv4")
find_package(OpenCV REQUIRED)
set(images_folder ${CMAKE_CURRENT_SOURCE_DIR}/images)

add_library(opencv_interface INTERFACE)
target_include_directories(opencv_interface INTERFACE ${OpenCV_INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}/inc")
target_link_libraries(opencv_interface INTERFACE ${OpenCV_LIBS})
target_compile_features(opencv_interface INTERFACE cxx_std_20)
target_compile_definitions(opencv_interface INTERFACE -DIMAGES_FOLDER="${images_folder}")

add_subdirectory(Week1)

add_subdirectory(Week3)
add_subdirectory(Week4)
add_subdirectory(Week5)
add_subdirectory(Week6_project1)
add_subdirectory(Week7)
add_subdirectory(Week8)