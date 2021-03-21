cmake_minimum_required(VERSION 3.11)
project(trackbar CXX)

# !!! Set OpenCV_DIR to your local Opencv installation
SET(OpenCV_DIR /home/kartik/opencv)
if (MSVC)
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif ()

find_package(OpenCV REQUIRED)
set(images_folder ${CMAKE_CURRENT_SOURCE_DIR}/Data)
file(GLOB sources *.cpp *.h)
add_executable(trackbar ${sources})
target_compile_features(trackbar PUBLIC cxx_std_17)
target_include_directories(trackbar PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(trackbar PUBLIC ${OpenCV_LIBS})
target_compile_definitions(trackbar PUBLIC -DDATA_PATH="${images_folder}")
