cmake_minimum_required(VERSION 2.6)
project(fsim)

set(CMAKE_BUILD_TYPE Debug)
cmake_policy(SET CMP0015 NEW)

include_directories(/home/dominykas/c++/vb01)
include_directories(/usr/include/freetype2)
include_directories(/usr/include/GLFW)
include_directories(/usr/include/boost)

link_directories(../vb01/build)
link_directories(/usr/lib)

set(core gameManager.cpp stateManager.cpp inputManager.cpp)
set(states abstractAppState.cpp guiAppState.cpp inGameAppState.cpp activeGameAppState.cpp)
set(gui button.cpp mainMenuButton.cpp exitButton.cpp optionsButton.cpp playButton.cpp loadButton.cpp textbox.cpp listbox.cpp)
set(content map.cpp playerData.h structure.cpp building.cpp unit.cpp aircraft.cpp jet.cpp helicopter.cpp)
set(utils defConfigs.h util.cpp)

add_executable(fsim main.cpp ${core} ${states} ${gui} ${content} ${utils})
target_link_libraries(fsim vb01)
target_link_libraries(fsim glfw)
target_link_libraries(fsim boost_system)
target_link_libraries(fsim boost_filesystem)
