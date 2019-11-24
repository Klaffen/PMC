//
// Created by VegarK on 30.10.2017.
//

#include <SFML/Window.hpp>
#include <Game/Board/Tile.h>
#include <Game/Board/Board.h>
#include <iostream>
#include "Input.h"

Tile *input::getMouseOverTile(std::vector<std::vector<Tile>> &tileList, sf::RenderWindow &window) {

    sf::Vector2f viewCoord = getMouse(window);
    sf::Vector2f tilePos((int)(viewCoord.x / Board::TILE_SIZE), (int)(viewCoord.y / Board::TILE_SIZE));
    Tile *retTile = &tileList[tilePos.x][tilePos.y];
    if (tileList.size() > tilePos.x && tileList.at(0).size() > tilePos.y && tilePos.x >= 0 && tilePos.y >= 0)
        return retTile;
    else
        return &tileList[0][0];
}

sf::Vector2f input::getMouse(sf::RenderWindow &window) {
    sf::Vector2i windowCoord = sf::Mouse::getPosition(window);
    sf::Vector2f viewCoord = window.mapPixelToCoords(windowCoord);
    return viewCoord;
}
