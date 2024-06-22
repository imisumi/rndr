cmake_minimum_required(VERSION 3.10)
project(Rndr-Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



set(FullOutputDir ${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${FullOutputDir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${FullOutputDir})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FullOutputDir})



set(RNDR_COURCE_DIR ${CMAKE_SOURCE_DIR}/Rndr/src)
set(SANDBOX_COURCE_DIR ${CMAKE_SOURCE_DIR}/Sandbox/src)


add_subdirectory(Rndr)
add_subdirectory(Sandbox)

