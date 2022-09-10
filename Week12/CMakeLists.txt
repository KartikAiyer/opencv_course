project(Week12 CXX)

add_library(week12_interface INTERFACE)
target_compile_definitions(week12_interface INTERFACE
        -DDATA_FOLDER=${CMAKE_CURRENT_SOURCE_DIR}/week12-cpp/data)
add_subdirectory(week12-cpp/cpp)