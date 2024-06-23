cmake_minimum_required(VERSION 3.10)
project(Rndr)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

message(STATUS "Building: ${PROJECT_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")

if (MSVC)
	# Use /MD (dynamic runtime) for Release configurations
	# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
	# set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MD")

	# Use /MDd (dynamic runtime for debugging) for Debug configurations
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MDd")
endif()




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
add_subdirectory(dep/glad)

# For now, we don't want GLFW to include any OpenGL headers
add_compile_definitions(GLFW_INCLUDE_NONE)

# add_library(${PROJECT_NAME} SHARED ${RNDR_SOURCES})
add_library(${PROJECT_NAME} STATIC ${RNDR_SOURCES})
# Link against GLFW
# target_include_directories(${PROJECT_NAME} PUBLIC glfw/include)


target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)

# Optionally specify include directories
target_include_directories(${PROJECT_NAME} PUBLIC glfw/include)
target_include_directories(${PROJECT_NAME} PUBLIC spdlog/include)
target_include_directories(${PROJECT_NAME} PUBLIC glm)
target_include_directories(${PROJECT_NAME} PUBLIC imgui)
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_COURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC glad/include)

target_include_directories(${PROJECT_NAME} PUBLIC ${RNDR_COURCE_DIR})
