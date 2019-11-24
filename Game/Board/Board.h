//
// Created by Roman Aleksandrov on 18/10/2017.
//

#ifndef DAT220_PROJECT_BOARD_H
#define DAT220_PROJECT_BOARD_H

#include <cmath>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Game/Board/Levels/Level.h"
#include "Game/Board/Tile.h"
#include "Game/Board/Shape3D.h"

class Board {

public:

    /**
     * Tile size used everywhere in the code as a reference
     */
    static const float TILE_SIZE;

    /**
     * Level default constructor
     */
    Level level;

    /**
     * Map size in pixels
     */
    sf::Vector2i mapSize;

    /**
     * Stores tiles
     */
    std::vector<std::vector<Tile>> tileMap;

    /**
     * Creates a tile map, executes drawing functions for tiles and obstacles
     * @param window
     */
    void init(sf::RenderWindow &window);

    /**
     * Converts float coordinates to a position in the tile map
     * @param position Tile position
     * @return Position of the tile in the tile map
     */
    sf::Vector2i getTileCoord(sf::Vector2f position);

    /**
     * Moves view up
     * @param window Reference to the game window
     * @param view Reference to the game view
     */
    void moveUp(sf::RenderWindow &window, sf::View & view);

    /**
     * Moves view down
     * @param window Reference to the game window
     * @param view Reference to the game view
     */
    void moveDown(sf::RenderWindow &window, sf::View & view);

    /**
     * Moves view left
     * @param window Reference to the game window
     * @param view Reference to the game view
     */
    void moveLeft(sf::RenderWindow &window, sf::View & view);

    /**
     * Moves view right
     * @param window Reference to the game window
     * @param view Reference to the game view
     */
    void moveRight(sf::RenderWindow &window, sf::View & view);

    /**
     * Draws tiles
     * @param window Reference to the game window
     */
    void drawTiles(sf::RenderWindow &window);

    /**
     * Draws obstacles
     * @param window Reference to the game window
     */
    void drawObstacles(sf::RenderWindow &window);

    /**
     * Sets a tile to taken
     * @param obstacles Reference to the list of obstacles
     * @param tileMap Reference to the tile map
     */
    void setTileTaken(std::vector<Shape3D> &obstacles, std::vector<std::vector<Tile>> &tileMap);

    /**
     * Sets a tile to free
     * @param obstacles Reference to the list of obstacles
     * @param tileMap Reference to the tile map
     */
    void setTileFree(std::vector<Shape3D> &obstacles, std::vector<std::vector<Tile>> &tileMap);

    /**
     * Processes keyboard inputs
     * @param window Reference to the game window
     * @param view Reference to the game view
     */
    void Process(sf::RenderWindow &window, sf::View &view);

private:

};

typedef std::shared_ptr<Board> BoardPtr;

#endif //DAT220_PROJECT_BOARD_H
