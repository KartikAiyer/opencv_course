project(Week13 CXX)

add_library(week13_interface INTERFACE)
target_compile_definitions(week13_interface INTERFACE
        -DDATA_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/week13-cpp/data")
add_subdirectory(week13-cpp/cpp)