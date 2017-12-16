//
// Created by sacha on 12/15/17.
//

#include <cstdio>
#include "Tile.h"


Tile::Tile() {
    this->top = 1;
    this->bot = 1;
    this->left = 1;
    this->right = 1;

    this->connection = unconnected;
}


// 0 - Empty
// 1 - Wall
bool Tile::isTraversed() {
    return this->connection  > 0;
    // /return (this->bot == 0 || this->top == 0 || this->left == 0 || this->right == 0);
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
