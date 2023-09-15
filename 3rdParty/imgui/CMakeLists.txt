cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(imgui)

add_library(imgui STATIC)
target_include_directories( imgui PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/backends
)