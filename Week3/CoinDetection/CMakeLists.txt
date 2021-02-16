project(coin_detection CXX)

file(GLOB cd_sources *.cpp *.h)

add_executable(coin_detect "${cd_sources}")
target_link_libraries(coin_detect PUBLIC opencv_interface week3_interface)
target_compile_features(coin_detect PUBLIC cxx_std_20)
