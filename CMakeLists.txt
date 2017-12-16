cmake_minimum_required(VERSION 3.7)
project(MazeCreator)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Tile.cpp Tile.h Maze.cpp Maze.h)
add_executable(MazeCreator ${SOURCE_FILES})