project(color_conversion_scratch CXX)

file(GLOB cc_sources *.cpp *.h)

add_executable(color_conversion_scratch "${cc_sources}")
target_link_libraries(color_conversion_scratch PUBLIC opencv_interface week4_interface)
target_compile_features(color_conversion_scratch PUBLIC cxx_std_20)
