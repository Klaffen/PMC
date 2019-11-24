//
// Created by VegarK on 30.10.2017.
//

#ifndef DAT220_PROJECT_INPUT_H
#define DAT220_PROJECT_INPUT_H


#include <SFML/Window.hpp>
#include "Game/Board/Tile.h"

class input {
public:
    static Tile *getMouseOverTile(std::vector<std::vector<Tile>> &tileList, sf::RenderWindow &window);
    static sf::Vector2f getMouse(sf::RenderWindow &window);
};


#endif //DAT220_PROJECT_INPUT_H
