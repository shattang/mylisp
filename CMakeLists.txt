cmake_minimum_required(VERSION 3.25)
project(MyLispProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(include)

# Add subdirectories
add_subdirectory(MyLisp)
add_subdirectory(MyLispRunner)
