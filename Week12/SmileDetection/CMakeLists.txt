project(smile_detect CXX)

file(GLOB source_files *.cpp *.h)
add_executable(smile_detect ${source_files})
target_link_libraries(smile_detect PRIVATE week12_interface opencv_interface dlib::dlib)
target_compile_features(smile_detect PRIVATE cxx_std_20)