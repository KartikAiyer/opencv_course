project(Project2 CXX)

file(GLOB sources *.cpp *.hpp)

add_library(project2_interface INTERFACE)
target_compile_definitions(project2_interface INTERFACE
        -DDATA_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/data")

add_executable(project2 "${sources}")
target_link_libraries(project2 PUBLIC project2_interface dlib::dlib opencv_interface)
target_compile_features(project2 PUBLIC cxx_std_20)

