@echo off

rem Set the OCIO environment variable to use a specific config URI
@REM set OCIO=ocio://default

rem Create a new directory called "build"
mkdir build

rem Change to the new directory
cd build

rem Run a command in the new directory (example: running CMake)
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Debug ^
	-DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake ^
	..
cmake --build . --config Debug


@REM cmake -A x64 -DCMAKE_BUILD_TYPE=Debug ..
@REM cmake --build . --config Debug




@REM cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake 