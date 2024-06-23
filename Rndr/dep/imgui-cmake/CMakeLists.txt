cmake_minimum_required(VERSION 3.10)
project(imgui)

# set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/../imgui)

set(IMGUI_SOURCES
	${IMGUI_COURCE_DIR}/imgui.h
	${IMGUI_COURCE_DIR}/imgui.cpp
	${IMGUI_COURCE_DIR}/imconfig.h
	${IMGUI_COURCE_DIR}/imgui_demo.cpp
	${IMGUI_COURCE_DIR}/imgui_draw.cpp
	${IMGUI_COURCE_DIR}/imgui_internal.h
	${IMGUI_COURCE_DIR}/imstb_rectpack.h
	${IMGUI_COURCE_DIR}/imstb_textedit.h
	${IMGUI_COURCE_DIR}/imstb_truetype.h
	${IMGUI_COURCE_DIR}/imgui_tables.cpp
	${IMGUI_COURCE_DIR}/imgui_widgets.cpp

	${IMGUI_COURCE_DIR}/backends/imgui_impl_glfw.h
	${IMGUI_COURCE_DIR}/backends/imgui_impl_glfw.cpp
	${IMGUI_COURCE_DIR}/backends/imgui_impl_opengl3.h
	${IMGUI_COURCE_DIR}/backends/imgui_impl_opengl3.cpp
	${IMGUI_COURCE_DIR}/backends/imgui_impl_opengl3_loader.h
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PRIVATE ${IMGUI_COURCE_DIR})
target_include_directories(imgui PRIVATE ${RNDR_COURCE_DIR}/../dep/glfw/include)
