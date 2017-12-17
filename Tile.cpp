//
// Created by sacha on 12/15/17.
//

#include <cstdio>
#include "Tile.h"


bool Tile::isTraversed() {
    return this->connection  > 0;
}

bool Tile::isConnected() {
    return this->connection == connected;
}


bool Tile::isTempConnected() {
    return this->connection == connected || this->connection == tempConnected;
}

bool Tile::isTempOrStuck() {
    return this->connection  == tempConnected || this->connection == stuck;
}
