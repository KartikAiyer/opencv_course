cmake_minimum_required(VERSION 3.11)

project(erosion_dilation CXX)

file(GLOB erosion_dilation_sources *.cpp *.h)

add_executable(erosion_dilation ${erosion_dilation_sources})
target_compile_features(erosion_dilation PUBLIC cxx_std_20)
target_link_libraries(erosion_dilation PUBLIC opencv_interface week3_interface)
