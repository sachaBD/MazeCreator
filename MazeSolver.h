//
// Created by sacha on 12/17/17.
//

#ifndef MAZECREATOR_MAZESOLVER_H
#define MAZECREATOR_MAZESOLVER_H


#include "Maze.h"

enum travelDirection {
    untraversed = 0,
    vertical = 1,
    horizontal = 2
};

class MazeSolver {

public:
    /**
     * Solve the given maze and print the solution
     *
     * @param maze
     * @return
     */
    static int** solve_maze(Maze* maze);

    static int** optimise_solution(Maze* maze, int** navigation);
};


#endif //MAZECREATOR_MAZESOLVER_H
