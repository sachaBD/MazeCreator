//
// Created by sacha on 12/15/17.
//

#ifndef MAZECREATOR_TITLE_H
#define MAZECREATOR_TITLE_H


enum TileState {
    unconnected = 0,
    connected = 1,
    stuck = 2,
    tempConnected = 3
};

enum Edge {
    open = 0,
    blocked = 1
};

class Tile {

public:
    int top = blocked;
    int bot = blocked;
    int left = blocked;
    int right = blocked;

    int connection = unconnected;

    bool isTraversed();

    bool isConnected();

    bool from(int dirFrom);

    bool isTempConnected();

    bool isTempOrStuck();
};


#endif //MAZECREATOR_TITLE_H
