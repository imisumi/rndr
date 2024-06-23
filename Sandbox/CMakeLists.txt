cmake_minimum_required(VERSION 3.10)
project(Sandbox)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

message(STATUS "Building: ${PROJECT_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "SANDBOX_COURCE_DIR: ${SANDBOX_COURCE_DIR}")

# Create a list of all C++ source files recursively under src/
file(GLOB_RECURSE SANDBOX_SOURCES 
	"${SANDBOX_COURCE_DIR}/*.cpp"
	"${SANDBOX_COURCE_DIR}/**/*.cpp"
	"${SANDBOX_COURCE_DIR}/**/**/*.cpp"
)

# Add the header file to the target sources
# target_sources(SANDBOX_SOURCES PRIVATE
#     ${RNDR_COURCE_DIR}/Rndr.h
# )

add_executable(Sandbox ${SANDBOX_SOURCES})

# Include directories for Sandbox target
target_include_directories(Sandbox PUBLIC
	${RNDR_COURCE_DIR}
	
	${RNDR_COURCE_DIR}/../dep/spdlog/include
	${RNDR_COURCE_DIR}/../dep/glm
	${RNDR_COURCE_DIR}/../dep/imgui
)

# target_link_options(Sandbox PRIVATE "/NODEFAULTLIB:MSVCRTD")

target_link_libraries(Sandbox PUBLIC Rndr)
# target_link_libraries(Sandbox PRIVATE "C:\\Users\\ichir\\Desktop\\rndr-engine\\bin\\Windows\\Debug\\Rndr.lib")