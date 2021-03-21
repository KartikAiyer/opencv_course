# Build Instructions

Uses the same style CMakeLists.txt file that is used by the course. Point to local openCV installation
using `OpenCV_DIR`.

Standard CMake build

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release .. 
make
```

The binary `trackbar` will be created in the build directory.

# Executable Provided

A prebuilt binary for Ubuntu 20.04 on an amd64 system is provided in `cmake-build-debug/trackbar`.
