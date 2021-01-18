project(ocv_week1 CXX)

set(images_path "${CMAKE_CURRENT_SOURCE_DIR}/week1-cpp/data/images")

add_library(week1_interface INTERFACE)
target_compile_definitions(week1_interface INTERFACE -DIMAGE_PATH="${images_path}")

add_subdirectory(sample_image_operations)
add_subdirectory(sunglass_filter)