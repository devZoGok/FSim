cmake_minimum_required(VERSION 2.6)
project(fsim)

include_directories(/home/dominykas/c++/vb01)
include_directories(/usr/include/freetype2)

link_directories(/home/dominykas/c++/vb01/build)

set(CMAKE_BUILD_TYPE Debug)

add_executable(fsim main.cpp)
target_link_libraries(fsim vb01)
