cmake_minimum_required(VERSION 3.11)

project(opencv_course CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(Conan)

set(CONAN_EXTRA_REQUIRES
    "spdlog/1.5.0"
    "docopt.cpp/0.6.3")
run_conan()

set(OpenCV_DIR "/home/kartik/opencv")

find_package( OpenCV REQUIRED )
set(images_folder ${CMAKE_CURRENT_SOURCE_DIR}/images)

add_library(opencv_interface INTERFACE )
target_include_directories(opencv_interface INTERFACE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(opencv_interface INTERFACE ${OpenCV_LIBS})
target_compile_features(opencv_interface INTERFACE cxx_std_20)
target_compile_definitions(opencv_interface INTERFACE -DIMAGES_FOLDER="${images_folder}")


add_subdirectory(Week1/sample_image_operations)
