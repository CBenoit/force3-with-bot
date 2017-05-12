cmake_minimum_required(VERSION 3.6)
project(force3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets REQUIRED)

qt5_wrap_ui(UI_FILES
        ui/force3.ui
        ui/gameboard.ui
        ui/mainmenu.ui
        ui/settingsmenu.ui)

add_executable(force3
        src/main.cpp
        src/force3.cpp
        src/gameboard.cpp
        src/mainmenu.cpp
        src/messagehandler.cpp
        src/gamesquare.cpp
        src/settingsmenu.cpp
        src/gamestate.cpp
        src/boardstate.cpp
        ${UI_FILES})

target_link_libraries(force3 Qt5::Widgets)
