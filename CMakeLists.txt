cmake_minimum_required(VERSION 3.26)
project(app)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src3/math)

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
        src3/main.cpp
        src3/window.cpp
        src3/engine/loader.cpp
        src3/engine/mesh.cpp
        src3/engine/renderer.cpp
        src3/math/m3pch.cpp
        src3/math/Vec3.cpp
        src3/math/Mat4.cpp
        src3/math/Scalar.cpp
        src3/math/Transform.cpp
        src3/math/Quat.cpp
        src3/engine/rasterizer.cpp
)


find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
target_link_libraries(app Qt6::Core Qt6::Gui Qt6::Widgets)
