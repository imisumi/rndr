cmake_minimum_required(VERSION 3.10)
project(imgui)

set(IMGUI_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.h
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/imconfig.h
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_internal.h
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_rectpack.h
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_textedit.h
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_truetype.h
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.h
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.h
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3_loader.h
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_include_directories(imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends)
# target_include_directories(imgui PRIVATE ${RNDR_COURCE_DIR}/../dep/glfw/include)