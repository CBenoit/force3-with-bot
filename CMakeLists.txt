cmake_minimum_required(VERSION 3.6)
project(force3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

include_directories(src/)

find_package(Qt5Widgets REQUIRED)

file(GLOB_RECURSE UI_SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui")

qt5_wrap_ui(UI_FILES
        ${UI_SOURCE_FILES})

file(GLOB_RECURSE SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(force3
        ${SOURCE_FILES}
        ${UI_FILES})

target_link_libraries(force3 Qt5::Widgets)
