//
// Created by sacha on 12/15/17.
//

#include <cstdio>
#include <cstdlib>
#include "Tile.h"

/**
 * Determines if the current tile is anything but unconnected.
 *
 * @return True if the tile is anything but unconnected, false otherwise.
 */
bool Tile::isTraversed() {
    return this->connection  > 0;
}

/**
 * Determines if the current tile is connected.
 *
 * @return True if this Tile is connected, false otherwise.
 */
bool Tile::isConnected() {
    return this->connection == connected;
}

/**
 * Determine if this Tile is connected or tempConnected.
 *
 * @return True if this Tile is connected or tempConnected, false otherwise.
 */
bool Tile::isTempConnected() {
    return this->connection == connected || this->connection == tempConnected;
}

/**
 * Determine if this Tile is tempConnected or stuck.
 *
 * @return Ture if this Tile if tempConnected or stuck, false otherwise.
 */
bool Tile::isTempOrStuck() {
    return this->connection  == tempConnected || this->connection == stuck;
}

/**
 * Left
 * Top
 * Right
 * Bot
 *
 * @param dir
 * @return
 */
int* Tile::can_move_dir(int dir) {
    // Result in order of x, y
    int* result = (int*) malloc(2 * sizeof(int));
    result[0] = NULL;
    result[1] = NULL;

    if(dir == 0 && this->left == open) {
        result[0] = -1;
        result[1] = 0;
    } else if(dir == 1 && this->top == open) {
        result[0] = 0;
        result[1] = -1;
    } else if(dir == 2 && this->right == open) {
        result[0] = 1;
        result[1] = 0;
    } else if(dir == 3 && this->bot == open) {
        //Cant re-enter the entry
        if(this->bot == entry) {
            return result;
        }

        result[0] = 0;
        result[1] = 1;
    }

    return result;
}