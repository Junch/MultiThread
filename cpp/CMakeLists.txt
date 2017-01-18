cmake_minimum_required(VERSION 3.0)

project(multithread)

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED)
#add_compile_options(-std=c++11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(lesson1)
add_subdirectory(lesson2)
add_subdirectory(lesson3)
add_subdirectory(lesson4)
