//
// Created by Roman Aleksandrov on 30/10/2017.
//

#ifndef DAT220_PROJECT_VISION_H
#define DAT220_PROJECT_VISION_H


#include <SFML/Graphics/RectangleShape.hpp>
#include <Game/Board/Board.h>
#include <algorithm>

class Vision {

public:

    /**
     * Default vision constructor
     */
    Vision();

    /**
     * Creates a vector with tiles visible from particular tile
     * @param tile Tile from which a vision should be assessed
     * @param visionRadius Radius of vision
     * @param board Reference to the game board
     * @return Returns vector of visible tiles
     */
    static std::vector<Tile> getVisibleTiles(Tile tile, int visionRadius, Board &board);

    /**
     * Checks whether tile is visible from particular tile
     * @param tile Tile from which assessment about whether another tile is visible or not should be made
     * @param visibleTiles List of visible tiles
     * @return Returns true if the tile is in sight
     */
    static bool tileIsInSight(Tile tile, std::vector<Tile> visibleTiles);

private:

};

#endif //DAT220_PROJECT_VISION_H
