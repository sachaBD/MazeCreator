#include <iostream>
#include <stdlib.h>
#include "Maze.h"
#include "MazeSolver.h"

Maze* create_maze(int height, int width) {
    srand(time(NULL));

    Maze* maze = new Maze(height, width);

    maze->create_initial_path();

    maze->print();

    maze->create_other_paths();

    maze->print();

    MazeSolver::solve_maze(maze);

    return maze;
}

int main(int argc, char** argv) {
    if(argc != 3) {
        std::cerr << "Please enter the maze dimensions in the form: width height" << std::endl;
        std::exit(1);
    }

    char* endInt;

    int width = strtol(argv[1], &endInt, 10);
    if(*endInt != 0 || width < 1) {
        std::cerr << "Invalid width entered." << std::endl;
        std::exit(1);
    }

    int height = strtol(argv[2], &endInt, 10);
    if(*endInt != 0 || width < 1) {
        std::cerr << "Invalid height entered." << std::endl;
        std::exit(1);
    }

    Maze* maze = create_maze(height, width);
    std::fflush(stdout);

    return 0;
}