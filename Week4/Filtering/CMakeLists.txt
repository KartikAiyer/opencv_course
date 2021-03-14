project(filtering_experiment CXX)

file(GLOB cc_sources *.cpp *.h)

add_executable(filtering_exp "${cc_sources}")
target_link_libraries(filtering_exp PUBLIC opencv_interface week4_interface)
target_compile_features(filtering_exp PUBLIC cxx_std_20)
