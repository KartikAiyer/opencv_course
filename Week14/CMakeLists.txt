project(Week14 CXX)

add_library(week14_interface INTERFACE)
target_compile_definitions(week14_interface INTERFACE
        -DDATA_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/data"
        -DVIDEO_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/data/videos"
        -DIMAGE_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/data/images"
        -DMODEL_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/data/models")
add_subdirectory(cpp)
