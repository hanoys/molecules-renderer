cmake_minimum_required(VERSION 3.26)
project(app)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src/math)
include_directories(src/engine)

list(APPEND COMPILE_OPTIONS
        -std=c++20
        -O2
        -fno-strict-overflow
        -Wno-deprecated-declarations
        -Wall
        -Wextra
        -Werror
        -Wno-unused-variable)


add_executable(app
        src/main.cpp
        src/window.cpp
        src/engine/loader.cpp
        src/engine/mesh.cpp
        src/engine/renderer.cpp
        src/math/m3pch.cpp
        src/math/Vec3.cpp
        src/math/Mat4.cpp
        src/math/Scalar.cpp
        src/math/Transform.cpp
        src/math/Quat.cpp
        src/engine/scene.cpp
        src/engine/render_manager.cpp
        src/visualization/resources.h
        src/visualization/resources.cpp
        src/visualization/molecula_generator.h
        src/visualization/molecula_generator.cpp
)

target_link_options(app PRIVATE
        -flto)
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
target_link_libraries(app Qt6::Core Qt6::Gui Qt6::Widgets)
