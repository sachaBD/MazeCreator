//
// Created by sacha on 12/15/17.
//

#include <cstdio>
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
