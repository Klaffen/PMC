//
// Created by Roman Aleksandrov on 31/10/2017.
//

#ifndef DAT220_PROJECT_LEVEL1_H
#define DAT220_PROJECT_LEVEL1_H


#include <vector>
#include <Game/Board/Shape3D.h>

class Level {

public:

    /**
     * Default level constructor
     */
    Level();

    /**
     * List of obstacles
     */
    std::vector<Shape3D> obstacles;

private:

    /**
     * Reflects all obstacles in top left quadrant of the game board over X axis
     */
    void reflectOverX();

    /**
     * Reflects all obstacles in top half over Y axis
     */
    void reflectOverY();

    /**
     * Reflects all obstacles located inside the triangle obtained by drawing a diagonal from bottom left corner over XY axis
     */
    void reflectOverXY();

};

#endif //DAT220_PROJECT_LEVEL1_H
