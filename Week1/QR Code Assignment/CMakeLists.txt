project(week1_qr_code CXX)

file(GLOB qr_sources *.cpp)

add_executable(week1_qr_code "${qr_sources}")
target_link_libraries(week1_qr_code PUBLIC opencv_interface week1_interface CONAN_PKG::spdlog CONAN_PKG::docopt.cpp)
target_compile_features(week1_qr_code PUBLIC cxx_std_20)
