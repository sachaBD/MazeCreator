//
// Created by sacha on 12/17/17.
//

#include <cstdio>
#include <zconf.h>
#include <cstdlib>
#include "MazeSolver.h"

//#define KRED  "\x1B[31m"
#define KRED  "\x1B[32m" //This is green
#define KNRM  "\x1B[0m"


int** MazeSolver::solve_maze(Maze *maze) {
    int x = 0;
    int y = maze->height - 1;

    // Find the entrance
    for (int w = 0; w < maze->width; w++) {
        if (maze->get_tile(w, y)->bot == open) {
            x = w;
            break;
        }
    }

    // Holds infomation on the traversal of the maze
    // 0 - untraversed
    // 1 - traversed vertically
    // 2 - traversed horizontally
    //int navigation[maze->width][maze->height];
    int** navigation = (int**) malloc(maze->width * sizeof(int*));
    for(int h=0; h < maze->height; h++) {
        navigation[h] = (int*) malloc(maze->width * sizeof(int));
    }


    navigation[x][y] = vertical;

    // Left
    // Top
    // Right
    // Bot
    int dir = 1;
    int *priorities;
    int *newDirs;
    int previousX = 99, previousY = 99;

    // Traverse the left most possible path that hasn't yet been traversed.
    while (y > -1) {
        printf("Dir: %d Inner x: %d y: %d\n", dir, x, y);
        //sleep(1);

        if (dir == 0) {
            int temp[4] = {1, 0, 3, 2};
            priorities = temp;
        } else if (dir == 1) {
            int temp[4] = {2, 1, 0, 3};
            priorities = temp;
        } else if (dir == 2) {
            int temp[4] = {3, 2, 1, 0};
            priorities = temp;
        } else if (dir == 3) {
            int temp[4] = {0, 3, 2, 1};
            priorities = temp;
        }

        for (int i = 0; i < 4; i++) {
            int *result = maze->get_tile(x, y)->can_move_dir(priorities[i]);

            printf("dir: %d res: %d %d\n", priorities[i], result[0], result[1]);

            if(y == maze->height - 1 && result[1] > 0) {
                continue;
            }

            if (result[0] != NULL || result[1] != NULL) {
                previousX = x;
                previousY = y;
                x += result[0];
                y += result[1];

                break;
            }
        }

        if (previousX < x) {
            dir = 2;
            navigation[previousX][previousY] = horizontal;
        } else if (previousX > x) {
            navigation[previousX][previousY] = horizontal;
            dir = 0;
        } else if (previousY > y) {
            navigation[previousX][previousY] = vertical;
            dir = 1;
        } else if (previousY < y) {
            navigation[previousX][previousY] = vertical;
            dir = 3;
        } else {
            printf("Error\n");
        }
    }




    printf("\n");

    //Print the top level
    for(int w=0; w < maze->width; w++) {
        if (maze->maze[w]->top != 0) {
            printf(" _");
        } else {
            printf("  ");
        }
    }
    printf("\n");

    for(int h=0; h < maze->height; h++) {
        printf("|");
        for(int w=0; w < maze->width; w++) {
            if(maze->maze[h * maze->width + w]->bot == blocked) {
                printf("_");
            } else {
                if(navigation[w][h] == vertical) {
                    printf("%s|%s",KRED, KNRM);
                } else if(navigation[w][h] == horizontal) {
                    printf("%s-%s", KRED, KNRM);
                } else {
                    printf(" ");
                }
            }

            if(maze->maze[h * maze->width + w]->right != 0) {
                printf("|");
            } else {
                if(navigation[w][h] == vertical) {
                    printf("%s|%s",KRED, KNRM);
                } else if(navigation[w][h] == horizontal) {
                    printf("%s-%s", KRED, KNRM);
                } else {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }

    return navigation;
}


int** MazeSolver::optimise_solution(Maze *maze, int **navigation) {
    // Go through the naviagation
    //
}
































