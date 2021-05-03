file(GLOB panorama_sources *.cpp *.h)

add_executable(assignment_pano "${panorama_sources}")
target_link_libraries(assignment_pano PUBLIC opencv_interface week7_interface)
target_compile_features(assignment_pano PUBLIC cxx_std_20)


