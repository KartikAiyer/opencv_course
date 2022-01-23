

add_library(week11_interface INTERFACE)
target_compile_definitions(week11_interface INTERFACE
        -DDATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/images"
        -DMODEL_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/models"
        -DRESULTS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/cpp/results")
add_subdirectory(cpp)