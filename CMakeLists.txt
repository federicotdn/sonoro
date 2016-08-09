cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(sonoro)

#set(CMAKE_BUILD_TYPE Release)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(warnings "-Wall")
set(compileflags "--std=c++11")

include_directories(include)
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(sonoro ${SOURCES})

find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings} ${compileflags}")
set(CMAKE_EXE_LINKER_FLAGS "-lportaudio -lfftw3f -lSDL2_ttf -lSDL2 -lGLEW")
