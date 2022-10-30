cmake_minimum_required(VERSION 3.16)
project(snake)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

add_executable(snake src/main.cpp)

target_link_libraries(snake sfml-graphics sfml-audio sfml-window sfml-system)