project(opencv_tutorial_example_image_operations CXX)

add_executable(sample_image_operations sample_image_operations.cpp)
target_link_libraries(sample_image_operations PUBLIC opencv_interface)
