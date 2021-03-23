file(GLOB camera_filter_sources *.cpp *.h)

add_executable(camera_filter "${camera_filter_sources}")
target_link_libraries(camera_filter PUBLIC opencv_interface project1_interface)
