project(autofocus CXX)

file(GLOB af_sources *.cpp *.h)

add_executable(autofocus "${af_sources}")
target_link_libraries(autofocus PUBLIC opencv_interface week4_interface)
target_compile_features(autofocus PUBLIC cxx_std_20)