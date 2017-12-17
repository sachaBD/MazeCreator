#include <iostream>
#include <stdlib.h>
#include "Maze.h"

Maze* create_maze(int height, int width) {
    srand(time(NULL));

    Maze* maze = new Maze(height, width);

    maze->create_initial_path();

    maze->print();

    maze->create_other_paths();

    maze->print();

    return maze;
}

int main(int argc, char** argv) {
    char* endInt;
    Maze* maze = create_maze(strtol(argv[1], &endInt, 10), strtol(argv[2], &endInt, 10));
    std::fflush(stdout);

    return 0;
}