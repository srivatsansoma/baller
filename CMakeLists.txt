cmake_minimum_required(VERSION 3.16)
project(baller)

set(CMAKE_CXX_STANDARD 20)

# Tell CMake where SFML config is
set(SFML_DIR /usr/lib/x86_64-linux-gnu/cmake/SFML)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(baller
        main.cpp
        ball.cpp
        rect_area.cpp
)

target_link_libraries(baller
        sfml-graphics
        sfml-window
        sfml-system
)
