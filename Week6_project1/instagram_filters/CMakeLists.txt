file(GLOB instagram_filter_sources *.cpp *.h)

add_executable(instagram_filters "${instagram_filter_sources}")
target_link_libraries(instagram_filters PUBLIC opencv_interface project1_interface)
target_compile_features(instagram_filters PUBLIC cxx_std_20)
