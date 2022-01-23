cmake_minimum_required(VERSION 2.8.12)

project(week11 CXX)

include(../dlib/dlib/cmake )
include_directories(${OpenCV_INCLUDE_DIRS})

MACRO(add_example name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} PUBLIC ${OpenCV_LIBS} dlib::dlib week11_interface)
  target_include_directories(${name} PUBLIC ${OpenCV_INCLUDE_DIRS})
ENDMACRO()

add_example(facialLandmarkDetector)
add_example(faceAlignment)
add_example(fastWebcamFLD)
add_example(trainFLD)
add_example(webcamStabilizedFLD)
