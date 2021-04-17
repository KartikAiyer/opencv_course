file(GLOB chroma_key_sources *.cpp *.h)

add_executable(chroma_key "${chroma_key_sources}")
target_compile_features(chroma_key PUBLIC cxx_std_20)
target_link_libraries(chroma_key PUBLIC project1_interface opencv_interface)
