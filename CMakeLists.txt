cmake_minimum_required(VERSION 3.22.1)
project(cg-labor4)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libs/glfw-3.3.8)
include_directories(
    libs/glad/include
    libs/glfw-3.3.8/deps
)

file(GLOB_RECURSE GLAD_SOURCES libs/glad/src/*.c)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)

add_executable(main main.cpp ${GLAD_SOURCES})
target_link_libraries(main OpenGL::GL glfw)
