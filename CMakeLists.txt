cmake_minimum_required(VERSION 3.26)
project(app)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src)

list(APPEND COMPILE_OPTIONS
        -std=c++20
        -O3
        -fno-strict-overflow
        -Wno-deprecated-declarations
        -Wall
        -Wextra
        -Werror
        -Wno-unused-variable)

add_executable(app
        src/main.cpp src/window.cpp src/scene.h src/scene.cpp src/graphics.cpp src/model.cpp
        src/triangle.cpp
        src/triangle.h
        src/primitives.h)



find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
target_link_libraries(app Qt6::Core Qt6::Gui Qt6::Widgets)
