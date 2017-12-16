//
// Created by sacha on 12/15/17.
//

#ifndef MAZECREATOR_MAZE_H
#define MAZECREATOR_MAZE_H

#include "Tile.h"

/*
 * 0 1 2
 * 3 4 5
 * 6 7 8
 * Strategy:
 *  - Create an initial path
 *  - If the path becomes hard stuck, mark the tile as stuck and traverse backwards, the only other traversed connection tile
 *
 */


class Maze {
public:
    Tile** maze;
    int height;
    int width;

    Maze(int height, int width);

    void create_initial_path();

    void create_other_paths();

    Tile* get_tile(int x, int y);
    bool is_surrounded(int x, int y);
    int* find_connected(int x, int y);

    void temp_to_connected();
    void stuck_to_connected();
    void stuck_to_temp();

    void print();
    void print_connections();

    bool is_temp_surrounded(int x, int y);
};


#endif //MAZECREATOR_MAZE_H
