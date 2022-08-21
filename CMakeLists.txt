cmake_minimum_required(VERSION 3.21)

project(SUDOKU LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(sudoku)

target_sources(sudoku
	PUBLIC include/sudoku_functions.hpp

	src/main.cpp
	src/sudoku_functions.cpp
)

target_compile_options(sudoku PRIVATE -Wall -Wextra -Wpedantic)

target_include_directories(sudoku PUBLIC include/)
target_link_libraries(sudoku -lsfml-graphics -lsfml-window -lsfml-system -lImGui-SFML)
