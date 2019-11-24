#ifndef DAT220_PROJECT_PATHFINDING_H
#define DAT220_PROJECT_PATHFINDING_H

#include <Game/Board/Board.h>
#include <Game/Board/Tile.h>
#include <queue>

class Pathfinding {

public:

    /**
     * Calculates the shortest path to each tile in movement range
     * @param start The tile that the unit stands on
     * @param maxMovement The maximum length a unit can move
     * @param tileMap The map from which you calculate paths
     * @return Returns a map of shortest path to each tile
     */
    static std::map<int, int> getMovementMap(Tile &start, int maxMovement,
                                             std::vector<std::vector<Tile>> &tileMap);
    /**
     * Construct a path form start to a given tile (goal)
     * @param start The tile the unit stands on
     * @param goal The tile the unit wants to move to
     * @param movementMap The information map of shortest path for each tile
     * @param maxMovement the maximum range of movement
     * @param tileMap the tile map / game board
     * @return Returns a path from start tile to goal tile
     */
    static std::vector<sf::Vector2i> getPath(Tile &start, Tile &goal, std::map<int, int> movementMap, int maxMovement,
                                             std::vector<std::vector<Tile>> &tileMap);
    /**
     * Get index of a given tile
     * @param position The tile you want to index
     * @return returns the index of the tile
     */
    static sf::Vector2i getTileIndex(Tile &position);
private:
    /**
     * Calculates the cost to move to a neighbour tile
     * @param tile The tile you check neighbours from
     * @param tileMap The tile map
     * @return returns the cost of moving to neighbours
     */
    static std::map<int , int> neighbourCost(Tile &tile,  std::vector<std::vector<Tile>> &tileMap);
    /**
     * Calculates the cost of moving from start to a given tile in the tile map
     * @param start the tile the unit stands on
     * @param maxMovement maximum range of movement
     * @param tileMap Map of tiles you can move to
     * @return returns a map with cost of moving to each tile
     */
    static std::map<int, std::map<int, int>> getTileGraph(Tile &start, int maxMovement, std::vector<std::vector<Tile>> &tileMap);
    /**
     * Finds the tile with the shortest distance in the distance map
     * @param nodes List of nodes to check
     * @param distance distance for each node
     * @return returns the node with shortest distance
     */
    static int findShortestDistance(std::vector<int> &nodes, std::map<int, int> distance);
    /**
     * Find the X and Y coordinates of a given tile
     * @param path List of tiles to get X and Y cooridinates
     * @param tileMap Tile map for game board
     * @return returns the coordinates for each tile in the path
     */
    static std::vector<sf::Vector2i> getTileCords(std::vector<int> &path, std::vector<std::vector<Tile>> &tileMap);
};

#endif //DAT220_PROJECT_PATHFINDING_H
