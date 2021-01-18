
file(GLOB sunglass_sources *.cpp *.h)

add_executable(sunglass "${sunglass_sources}")
target_link_libraries(sunglass PUBLIC opencv_interface week1_interface)
target_compile_features(sunglass PUBLIC cxx_std_20)