//
// Created by Roman Aleksandrov on 18/10/2017.
//

#ifndef DAT220_PROJECT_TILE_H
#define DAT220_PROJECT_TILE_H

#include <SFML/Graphics.hpp>

class Tile: public sf::RectangleShape{

public:

    /**
     * Default tile constructor
     * @param position Position of the tile
     * @param tileSize Size of the tile
     */
    Tile(sf::Vector2f position, int tileSize);

    /**
     * Tile id
     */
    const int id;

    /**
     * True if tile is visible
     */
    int isVisible;

    /*
     * True if tile is taken
     */
    bool isTaken;

    /**
     * True if tile has unit
     */
    bool hasUnit;

    /**
     * Sets the color of the tile to highlighted
     */
    void setHighlighted();

    /**
     * Sets the color of the tile to not highlighted
     */
    void setNotHighlighted();

private:

    /**
     * Generates tile id
     */
    static int nextId;

    /**
     * Outline thickness
     */
    float outline;

    /**
     * Outline color
     */
    sf::Color outlineColor;

    /*
     * Tile color for not highlighted
     */
    sf::Color notHighlighted;

    /**
     * Tile color for highlighted
     */
    sf::Color highlighted;

};

bool operator==(Tile &tile1, const Tile &tile2);

#endif //DAT220_PROJECT_TILE_H