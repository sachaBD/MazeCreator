//
// Created by sacha on 12/15/17.
//

#ifndef MAZECREATOR_MAZE_H
#define MAZECREATOR_MAZE_H

#include "Tile.h"


class Maze {
public:
    int height;
    int width;
    Tile** maze;

    Maze(int height, int width);

    Tile* get_tile(int x, int y);

    void create_initial_path();
    void create_other_paths();

    void print();
    void print_connections();

private:

    bool is_surrounded(int x, int y);
    bool is_temp_surrounded(int x, int y);
    int* find_connected(int x, int y);

    void temp_to_connected();
    void stuck_to_connected();
    void stuck_to_unconnected();
};


#endif //MAZECREATOR_MAZE_H
