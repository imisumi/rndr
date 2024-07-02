#!/bin/bash

# Create a new directory called "build"
mkdir -p build

# Change to the new directory
cd build

# Run a command in the new directory (example: running CMake)
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug .. \
	-DCMAKE_TOOLCHAIN_FILE=/home/imisumi/sgoinfre/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Debug
# make
