@echo off

mkdir build

cd build

cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Debug ^
	-DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake ^
	..
@REM cmake --build . --config Debug
cmake --build . --config Debug -- /m:8

