//
// Created by sacha on 12/15/17->

#include <cstdlib>
#include <iostream>
#include <sys/time.h>
#include "Maze.h"
#include "Tile.h"


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

void Maze::create_initial_path(){
    // start at a random position on the bottom of the maze
    int x = rand() % this->width;
    int y = this->height - 1;

    // Enter the maze
    this->get_tile(x, y)->bot = open;
    this->get_tile(x, y)->connection = connected;

    // Traverse the maze until the end of the top of the maze is reached.
    while(y > -1) {
        // printf("%d, %d\n", x, y);
        if(this->is_surrounded(x, y)) {
            this->get_tile(x, y)->connection = stuck;

            //Move backwards to the other connection tile
            int* r = find_connected(x, y);
            x = r[0];
            y = r[1];
        }

        // Choose a direction to travel
        // 0 - top
        // 1 - bot
        // 2 - left
        // 3 - right
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
    this->stuck_to_connected();
}


void Maze::create_other_paths() {
    // Loop over each location
    for(int i=0; i < this->height * this->width; i++) {
        int x = i % this->width;
        int y = i / this->width;
        //printf("i: %d x: %d y: %d\n", i, x, y);

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
                printf("is surrounded.\n");

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
                    printf("Should never get here: x:%d y%d. \n",x , y);
                    this->print();
                    this->print_connections();
                    this->stuck_to_temp();
                }

                continue;
            }

            /*
             * 0 - top
             * 1 - bot
             * 2 - left
             * 3 - right
             */
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
            } else {
//                printf("x: %d, y: %d\n", x, y);
//                this->print();
//                this->print_connections();
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



// 0 1 2
// 3 4 5
// 6 7 8

void Maze::print() {
    printf("\n\n");

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

Tile* Maze::get_tile(int x, int y) {
    return this->maze[y * this->width + x];
}

int* Maze::find_connected(int x, int y) {
    int* result = (int*) malloc(2 * sizeof(int));
    result[0] = -1;
    result[1] = -1;

    if (y != 0 && this->get_tile(x, (y - 1))->isTempConnected()) {
        result[0] = x;
        result[1] = y - 1;
    } else if(y != this->height - 1 && this->get_tile(x, y + 1)->isTempConnected()) {
        result[0] = x;
        result[1] = y + 1;
    } else if(x != 0 && this->get_tile(x - 1, y)->isTempConnected()) {
        result[0] = x - 1;
        result[1] = y;
    } else if(x != this->width - 1 && this->get_tile(x + 1, y)->isTempConnected()) {
        result[0] = x + 1;
        result[1] = y;
    }

    return result;
}

void Maze::stuck_to_connected() {
    for(int i=0; i < this->height * this->width; i++) {
        if(this->maze[i]->connection == stuck) {
            this->maze[i]->connection = connected;
        }
    }
}

void Maze::temp_to_connected() {
    for(int i=0; i < this->height * this->width; i++) {
        if(this->maze[i]->connection == tempConnected) {
            this->maze[i]->connection = connected;
        }
    }
}


void Maze::print_connections() {
    for(int h=0; h < this->height; h++) {
        for(int w=0; w < this->width; w++) {
            printf("%d", this->get_tile(w, h)->connection);
        }

        printf("\n");
    }
}

void Maze::stuck_to_temp() {
    for(int i=0; i < this->height * this->width; i++) {
        if(this->maze[i]->connection == stuck) {
            this->maze[i]->connection = tempConnected;
        }
    }
}