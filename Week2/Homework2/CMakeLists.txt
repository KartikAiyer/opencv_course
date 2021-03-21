cmake_minimum_required(VERSION 3.11)
project(mouse_annotation CXX)

# !!! Set OpenCV_DIR to your local Opencv installation
SET(OpenCV_DIR /home/kartik/opencv)
if (MSVC)
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif ()

find_package(OpenCV REQUIRED)
set(images_folder ${CMAKE_CURRENT_SOURCE_DIR}/Data)

file(GLOB sources *.cpp *.h)
add_executable(mouse_annotation ${sources})
target_compile_features(mouse_annotation PUBLIC cxx_std_17)
target_include_directories(mouse_annotation PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(mouse_annotation PUBLIC ${OpenCV_LIBS})
target_compile_definitions(mouse_annotation PUBLIC -DDATA_PATH="${images_folder}")
