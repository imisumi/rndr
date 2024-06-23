cmake_minimum_required(VERSION 3.10)
project(glad VERSION 1.0 LANGUAGES C)

# Glad library sources
add_library(glad STATIC src/glad.c)

# Include directories
target_include_directories(glad PUBLIC include)

# Optionally, set C standard explicitly (if needed)
# set_target_properties(glad PROPERTIES C_STANDARD 11)

# Optionally, set compiler-specific options or flags
# For example:
# if(MSVC)
#   target_compile_options(glad PRIVATE /W4 /WX)
# else()
#   target_compile_options(glad PRIVATE -Wall -Wextra -pedantic)
# endif()
