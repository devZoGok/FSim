cmake_minimum_required(VERSION 2.6)
project(fsim)

include_directories(/home/dominykas/c++/vb01)
include_directories(/usr/include/freetype2)
include_directories(/usr/include/GLFW)

link_directories(/home/dominykas/c++/vb01/build)

set(CMAKE_BUILD_TYPE Debug)
set(core gameManager.cpp stateManager.cpp inputManager.cpp)
set(states abstractAppState.cpp guiAppState.cpp)
set(gui button.cpp)
set(utils defConfigs.h)

add_executable(fsim main.cpp ${core} ${states} ${gui} ${utils})
target_link_libraries(fsim vb01)
target_link_libraries(fsim glfw)
