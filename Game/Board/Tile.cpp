//
// Created by Roman Aleksandrov on 18/10/2017.
//

#include "Tile.h"
int Tile::nextId = 1;

Tile::Tile(sf::Vector2f position, int tileSize) : id(nextId++) {

    isVisible = false;
    isTaken = false;
    hasUnit = false;

    outline = -0.5;
    notHighlighted = sf::Color(12, 0, 42);
    highlighted = sf::Color(255, 197, 154, 50);
    outlineColor = sf::Color(246, 28, 62);

    setPosition(position);
    setSize(sf::Vector2f(tileSize, tileSize));
    setFillColor(notHighlighted);
    setOutlineThickness(outline);
    setOutlineColor(outlineColor);
}

void Tile::setHighlighted() {
    this->setFillColor(highlighted);
}

void Tile::setNotHighlighted() {
    this->setFillColor(notHighlighted);
}

bool operator==(Tile &tile1, const Tile &tile2){
    return tile1.id == tile2.id;
}