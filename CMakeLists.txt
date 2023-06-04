cmake_minimum_required(VERSION 3.25)
project(pruebaopengl)


set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED COMPONENTS SDL2)

include_directories(glad/include)

add_executable(pruebaopengl glad/src/gl.c main.cpp )

target_link_libraries(pruebaopengl PRIVATE SDL2::SDL2)