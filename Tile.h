//
// Created by sacha on 12/15/17.
//

#ifndef MAZECREATOR_TITLE_H
#define MAZECREATOR_TITLE_H


enum TileState {
    unconnected = 0,
    connected = 1,
    stuck = 2,
    tempConnected = 3,
    tempStuck = 4
};

class Tile {

public:
    // Edges
    // 0 - Open
    // 1 - Closed
    // 2 - Maze edge
    int top = 1;
    int bot = 1;
    int left = 1;
    int right = 1;

    int connection = 0;

    Tile();

    bool isTraversed();
    bool isConnected();

    bool from(int dirFrom);

    bool isTempConnected();

    bool isTempOrStuck();
};


#endif //MAZECREATOR_TITLE_H
