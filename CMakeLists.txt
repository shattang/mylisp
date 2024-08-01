cmake_minimum_required(VERSION 3.10)
project(MyLispProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Add subdirectories
add_subdirectory(MyLisp)
add_subdirectory(MyLispRunner)
