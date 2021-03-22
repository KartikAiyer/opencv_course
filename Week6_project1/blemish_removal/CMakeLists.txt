file(GLOB blemish_sources *.cpp *.h)

add_executable(blemish "${blemish_sources}")
target_link_libraries(blemish PUBLIC opencv_interface project1_interface)
target_compile_features(blemish PUBLIC cxx_std_20)
