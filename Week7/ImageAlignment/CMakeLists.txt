file(GLOB image_alignment_sources *.cpp *.h)

add_executable(image_alignment "${image_alignment_sources}")
target_link_libraries(image_alignment PUBLIC opencv_interface week7_interface)
target_compile_features(image_alignment PUBLIC cxx_std_20)

