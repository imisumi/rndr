
cmake_minimum_required(VERSION 3.20)
project(Render-Engine LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Set CMAKE_TOOLCHAIN_FILE to vcpkg's toolchain file
set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")

# print build configuration
if (NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected, default to Release")
	set(CMAKE_BUILD_TYPE "Release")
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")



set(RNDR_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Rndr/src)
set(SANDBOX_COURCE_DIR ${CMAKE_SOURCE_DIR}/sandbox/src)
set(EDITOR_COURCE_DIR ${CMAKE_SOURCE_DIR}/Editor/src)

set(FullOutputDir ${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${FullOutputDir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${FullOutputDir})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FullOutputDir})

# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")

# RNDR_ENABLE_ASSERTS
set(RNDR_ENABLE_ASSERTS ON)

if(UNIX AND NOT APPLE)
	add_compile_definitions(RNDR_PLATFORM_LINUX)
elseif(WIN32)
	add_compile_definitions(RNDR_PLATFORM_WINDOWS)
	message(STATUS "Windows platform detected")
endif()

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(OpenImageIO CONFIG REQUIRED)
find_package(OpenColorIO CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)


add_subdirectory(Rndr/dep/imgui)
# add_subdirectory(Rndr/dep/yaml-cpp)

file(GLOB_RECURSE ALL_SOURCES 
"${RNDR_SOURCE_DIR}/*.cpp"
"${RNDR_SOURCE_DIR}/**/*.cpp"
"${RNDR_SOURCE_DIR}/**/**/*.cpp"

"${EDITOR_COURCE_DIR}/*.cpp"
"${EDITOR_COURCE_DIR}/**/*.cpp"
"${EDITOR_COURCE_DIR}/**/**/*.cpp"
)

add_executable(${PROJECT_NAME} ${ALL_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE imgui)
# target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
	OpenGL::GL
	spdlog::spdlog
	glfw
	glad::glad
	glm::glm
	yaml-cpp::yaml-cpp
	OpenImageIO::OpenImageIO
	OpenColorIO::OpenColorIO
	assimp::assimp
)
# target_include_directories(${PROJECT_NAME} PRIVATE yaml-cpp/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${RNDR_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/Rndr/dep/imgui
	${CMAKE_CURRENT_SOURCE_DIR}/Rndr/dep/imgui/imgui
	${CMAKE_CURRENT_SOURCE_DIR}/Rndr/dep
	# ${CMAKE_CURRENT_SOURCE_DIR}/Rndr/dep/ImGuizmo
)

