cmake_minimum_required(VERSION 3.10)
project(Rndr)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

message(STATUS "Building: ${PROJECT_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")


if(UNIX AND NOT APPLE)
	add_compile_definitions(RNDR_PLATFORM_LINUX)
elseif(WIN32)
	add_compile_definitions(RNDR_PLATFORM_WINDOWS)
endif()
add_compile_definitions(RNDR_BUILD_DLL)
# add_compile_definitions(GLFW_INCLUDE_NONE)

message(STATUS "RNDR_COURCE_DIR: ${RNDR_COURCE_DIR}")



# Create a list of all C++ source files recursively under src/
file(GLOB_RECURSE RNDR_SOURCES 
    "${RNDR_COURCE_DIR}/*.cpp"
    "${RNDR_COURCE_DIR}/**/*.cpp"
    "${RNDR_COURCE_DIR}/**/**/*.cpp"
)


# add_library(${PROJECT_NAME} SHARED ${RNDR_SOURCES})


set(IMGUI_COURCE_DIR ${RNDR_COURCE_DIR}/../dep/imgui)

# Enable position-independent code for all targets
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Add GLFW as a subdirectory
add_subdirectory(dep/glfw)
add_subdirectory(dep/spdlog)
add_subdirectory(dep/glm)
add_subdirectory(dep/imgui-cmake)

add_library(${PROJECT_NAME} SHARED ${RNDR_SOURCES})
# Link against GLFW
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

# Optionally specify include directories
target_include_directories(${PROJECT_NAME} PRIVATE glfw/include)
target_include_directories(${PROJECT_NAME} PRIVATE spdlog/include)
target_include_directories(${PROJECT_NAME} PRIVATE glm)
target_include_directories(${PROJECT_NAME} PRIVATE imgui)
