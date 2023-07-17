cmake_minimum_required(VERSION 3.17)
project(app)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src)

list(APPEND COMPILE_OPTIONS
        -std=c++20
        -fno-strict-overflow
        -Wno-deprecated-declarations
        -Wall
        -Wextra
        -Werror
        -Wno-unused-variable)

add_executable(app
        src/main.cpp src/window.cpp src/scene.h src/scene.cpp src/graphics.cpp src/model.cpp src/my_math.cpp)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
target_link_libraries(app Qt5::Core Qt5::Gui Qt5::Widgets)

