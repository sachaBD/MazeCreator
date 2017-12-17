//
// Created by sacha on 12/15/17->

#include <cstdlib>
#include <iostream>
#include "Maze.h"


Maze::Maze(int height, int width) {
    this->height = height;
    this->width = width;

    //Create the array
    this->maze = (Tile**)malloc(height * width * sizeof(Tile*));

    for(int w=0; w < width; w++) {
        for(int h=0; h < height; h++) {
            // Add each entry to the maze
            this->maze[h * this->width + w] = new Tile();
        }
    }
}

/**
 * Creates an initial path from the bottom of the maze to the top. This is created by taking a random walk travelling
 * in a random direction at each Tile. In the case that the current Tile is completely surrounded by connected tiles,
 * the path is retraced until a valid move can be made.
 *
 * The current direction are used:
 * 0 - top
 * 1 - bot
 * 2 - left
 * 3 - right
 */
void Maze::create_initial_path(){
    // start at a random position on the bottom of the maze
    int x = rand() % this->width;
    int y = this->height - 1;

    // Enter the maze
    this->get_tile(x, y)->bot = open;
    this->get_tile(x, y)->connection = connected;

    // Traverse the maze until the end of the top of the maze is reached.
    while(y > -1) {
        if(this->is_surrounded(x, y)) {
            this->get_tile(x, y)->connection = stuck;

            // Move backwards to the other connection tile
            int* r = find_connected(x, y);

            // Close the opened path
            // Find the direction that we are moving back to
            if(r[1] == y - 1) {
                // Moving upwards, close top of current and bottom of next
                this->get_tile(x, y)->top = blocked;
                this->get_tile(x, y - 1)->bot = blocked;
            } else if(r[1] == y + 1) {
               // Moved downwards
                this->get_tile(x, y)->bot = blocked;
                this->get_tile(x, y + 1)->top = blocked;
            } else if(r[0] == x - 1) {
                // Moving to the left
                this->get_tile(x, y)->left = blocked;
                this->get_tile(x - 1, y)->right = blocked;
            } else if(r[0] == x + 1) {
                // Moving to the right
                this->get_tile(x, y)->right = blocked;
                this->get_tile(x + 1, y)->left = blocked;
            } else {
              throw "Path error.\n";
            }

            x = r[0];
            y = r[1];
        }

        int dir = rand() % 4;

        if(dir == 0) {
            if (y == 0) {
                this->get_tile(x, y)->top = open;
                this->get_tile(x, y)->connection = connected;
                break;
            }

            // Check if it has already been traversed to
            if (this->get_tile(x, y - 1)->isTraversed()) {
                continue;
            }

            // Open the relevent edge
            this->get_tile(x, y)->top = 0;
            this->get_tile(x, y - 1)->bot = 0;
            this->get_tile(x, y - 1)->connection = connected;
            // Move the current position
            y -= 1;
        } else if(dir == 1 && y != this->height - 1 && ! this->get_tile(x, y + 1)->isTraversed()) {
            this->get_tile(x, y)->bot = 0;

            y += 1;
            this->get_tile(x, y)->top = 0;
            this->get_tile(x, y)->connection = connected;
        } else if(dir == 2) {
            if(x == 0) {
                continue;
            }

            if(this->get_tile(x - 1, y)->isTraversed()) {
                continue;
            }

            this->get_tile(x, y)->left = 0;
            this->get_tile(x - 1, y)->right = 0;
            this->get_tile(x - 1, y)->connection = connected;
            x -= 1;
        } else if(dir == 3) {
            if(x == this->width - 1) {
                continue;
            }

            if(this->get_tile(x + 1, y)->isTraversed()) {
                continue;
            }

            this->get_tile(x, y)->right = 0;
            this->get_tile(x + 1, y)->left = 0;
            this->get_tile(x + 1, y)->connection = connected;
            x += 1;
        }
    }

    // Replace all stuck tiles with connected
    this->stuck_to_unconnected();
}

/**
 * Creates a path from each point which connects to both the start and end of the maze, however these paths cannot
 * form a loop.
 *
 * After this method there is still only a single path with no backtracing from the start to end of the maze.
 */
void Maze::create_other_paths() {
    // Loop over each location
    for(int i=0; i < this->height * this->width; i++) {
        int x = i % this->width;
        int y = i / this->width;

        // Check if this location is connected
        if(this->get_tile(x, y)->isConnected()) {
            continue;
        }

        // If not set this location as tempConnected
        this->get_tile(x, y)->connection = tempConnected;

        while(true) {
            // Termination condition: current location is connected
            if(this->get_tile(x, y)->isConnected()) {
                break;
            }

            // Move a random direction that is not:
            // - Temp connected
            // - Off the board
            // then:
            // if it is not already marked mark it as temp Connected
            //
            // Check if the current tile is surrounded, if so:
            // Mark the current tile as stuck
            // Move to a tempConnected Tile
            // After finding a connected tile, reset all stuck to connected

            if(this->is_temp_surrounded(x, y)) {
                // Mark as stuck
                this->get_tile(x, y)->connection = stuck;

                // Move to a tile that is tempConnected in the order: top, right, bot, left
                if(y != 0 && this->get_tile(x, y - 1)->connection == tempConnected) {
                    y = y - 1;
                } else if(x != this->width - 1 && this->get_tile(x + 1, y)->connection == tempConnected) {
                    x = x + 1;
                } else if(x != 0 && this->get_tile(x - 1, y)->connection == tempConnected) {
                    x = x - 1;
                } else if(y != this->height - 1 && this->get_tile(x, y + 1)->connection == tempConnected) {
                    y = y + 1;
                } else {
                    throw "Path Error.\n";
                }

                continue;
            }

            int dir = rand() % 4;

            // Check if a move upwards is valid
            if(dir == 0 && y != 0 && !this->get_tile(x, y - 1)->isTempOrStuck()) {
                // Traverse to the top
                this->get_tile(x, y)->top = 0;

                y = y - 1;
                this->get_tile(x, y)->bot = 0;
            } else if(dir == 1 && y != this->height - 1 && !this->get_tile(x, y + 1)->isTempOrStuck()) {
                this->get_tile(x, y)->bot = 0;

                y = y + 1;
                this->get_tile(x, y)->top = 0;
            } else if(dir == 2 && x != 0 && !this->get_tile(x - 1, y)->isTempOrStuck()) {
                this->get_tile(x, y)->left = 0;

                x = x - 1;
                this->get_tile(x, y)->right = 0;
            } else if(dir == 3 && x != this->width - 1 && !this->get_tile(x + 1, y)->isTempOrStuck()) {
                this->get_tile(x, y)->right = 0;

                x = x + 1;
                this->get_tile(x, y)->left = 0;
            }

            // Mark as temp connected or end
            if(this->get_tile(x, y)->isConnected()) {
                break;
            } else {
                this->get_tile(x, y)->connection = tempConnected;
            }

        }

        // Reset all stuck to connected
        this->stuck_to_connected();
        this->temp_to_connected();
    }
}

/**
 * Determines if the given Tile is completely surrounded by connected or stuck Tiles.
 *
 * @param x The x location of the Tile to check.
 * @param y The y location of the Tile to check.
 * @return True if the Tile is completely surrounded, False otherwise.
 */
bool Maze::is_surrounded(int x, int y){
    if(
            (this->maze[y * this->width + x]->isTraversed()) &&
            (y == 0 || this->maze[(y - 1) * this->width + x]->isTraversed()) &&
            (y == this->height - 1 || this->maze[(y + 1) * this->width + x]->isTraversed()) &&
            (x == 0 || this->maze[y * this->width + (x - 1)]->isTraversed()) &&
            (x == this->width - 1 || this->maze[y * this->width + (x + 1)]->isTraversed())
            ) {
        return true;
    }

    return false;
}

/**
 * Determines if the given Tile is completely surrounded by temporarily connected or stuck Tiles.
 *
 * @param x The x location of the Tile to check.
 * @param y The y location of the Tile to check.
 * @return True if the Tile is completely surrounded, False otherwise.
 */
bool Maze::is_temp_surrounded(int x, int y) {
    if(     (y == 0 || this->get_tile(x, y - 1)->isTempOrStuck()) &&
            (y == this->height - 1 || this->get_tile(x, y + 1)->isTempOrStuck()) &&
            (x == 0 || this->get_tile(x - 1, y)->isTempOrStuck()) &&
            (x == this->width - 1 || this->get_tile(x + 1, y)->isTempOrStuck())
            ) {
        return true;
    }

    return false;
}


/**
 * Displays the maze on the command line.
 */
void Maze::print() {
    printf("\n");

    //Print the top level
    for(int w=0; w < this->width; w++) {
        if (this->maze[w]->top != 0) {
            printf(" _");
        } else {
            printf("  ");
        }
    }
    printf("\n");

    for(int h=0; h < this->height; h++) {
        printf("|");
        for(int w=0; w < this->width; w++) {
            if(this->maze[h * this->width + w]->bot != 0) {
                printf("_");
            } else {
                printf(" ");
            }

            if(this->maze[h * this->width + w]->right != 0) {
                printf("|");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

/**
 * Displays the connection status of each tile within the maze.
 */
void Maze::print_connections() {
    for(int h=0; h < this->height; h++) {
        for(int w=0; w < this->width; w++) {
            printf("%d", this->get_tile(w, h)->connection);
        }

        printf("\n");
    }
}

/**
 * Returns the tile at the given x and y location.
 *
 * @param x The x location of the Tile to return.
 * @param y The y location of the Tile to return.
 * @return The Tile at the given location.
 */
Tile* Maze::get_tile(int x, int y) {
    return this->maze[y * this->width + x];
}

/**
 * Finds a tile connected to the Tile at the given location and returns its location.
 *
 * @param x The x location of the Tile to find a connection to.
 * @param y The y location of the Tile to find a connection to.
 * @return An array of 2 ints which are the location of the connected Tile in the order x, y.
 */
int* Maze::find_connected(int x, int y) {
    int* result = (int*) malloc(2 * sizeof(int));
    result[0] = -1;
    result[1] = -1;

    // Check if there is an open path
    if (y != 0 && this->get_tile(x, (y - 1))->isTempConnected() && this->get_tile(x, y)->top == open) {
        result[0] = x;
        result[1] = y - 1;
    } else if(y != this->height - 1 && this->get_tile(x, y + 1)->isTempConnected() && this->get_tile(x, y)->bot == open) {
        result[0] = x;
        result[1] = y + 1;
    } else if(x != 0 && this->get_tile(x - 1, y)->isTempConnected() && this->get_tile(x, y)->left == open) {
        result[0] = x - 1;
        result[1] = y;
    } else if(x != this->width - 1 && this->get_tile(x + 1, y)->isTempConnected() && this->get_tile(x, y)->right == open) {
        result[0] = x + 1;
        result[1] = y;
    }

    return result;
}

/**
 * Change all stuck Tiles to the connected Tiles.
 */
void Maze::stuck_to_connected() {
    for(int i=0; i < this->height * this->width; i++) {
        if(this->maze[i]->connection == stuck) {
            this->maze[i]->connection = connected;
        }
    }
}

/**
 * Change all stuck Tiles to unconnected Tiles.
 */
void Maze::stuck_to_unconnected() {
    for(int i=0; i < this->height * this->width; i++) {
        if(this->maze[i]->connection == stuck) {
            this->maze[i]->connection = unconnected;
        }
    }
}

/**
 * Change all temporarily connected Tiles to connected Tiles.
 */
void Maze::temp_to_connected() {
    for(int i=0; i < this->height * this->width; i++) {
        if(this->maze[i]->connection == tempConnected) {
            this->maze[i]->connection = connected;
        }
    }
}
