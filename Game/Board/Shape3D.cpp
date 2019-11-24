//
// Created by Roman Aleksandrov on 18/10/2017.
//

#include <Game/Board/Shape3D.h>
#include "Board.h"

Shape3D::Shape3D(sf::Vector2f size, sf::Vector2f position, float height) {

    this->size = {size.x*Board::TILE_SIZE, size.y*Board::TILE_SIZE};
    this->position = {position.x*Board::TILE_SIZE, position.y*Board::TILE_SIZE};
    this->height = height;

    outlineThickness = -1;

    topColor = sf::Color(21, 59, 112);
    wallTopColor = sf::Color(60, 77, 123);
    wallBaseColor = sf::Color::Red;
    outlineColor = sf::Color(183, 246, 253);

    base.setSize({size.x*Board::TILE_SIZE, size.y*Board::TILE_SIZE});
    base.setPosition({position.x*Board::TILE_SIZE, position.y*Board::TILE_SIZE});

    top.setFillColor(topColor);
    top.setOutlineThickness(outlineThickness);
    top.setOutlineColor(outlineColor);

    transparentRect.setFillColor(topColor);

    rear.setPrimitiveType(sf::Quads);
    rear.resize(4);
    rear[0].position = sf::Vector2f(top.getPosition());
    rear[1].position = sf::Vector2f(top.getPosition().x + top.getSize().x, top.getPosition().y);
    rear[2].position = sf::Vector2f(base.getPosition().x + base.getSize().x, base.getPosition().y);
    rear[3].position = sf::Vector2f(base.getPosition());
    rear[0].color = wallTopColor;
    rear[1].color = wallTopColor;
    rear[2].color = wallBaseColor; // base point
    rear[3].color = wallBaseColor; // base point

    right.setPrimitiveType(sf::Quads);
    right.resize(4);
    right[0].position = sf::Vector2f(base.getPosition().x + base.getSize().x, base.getPosition().y);
    right[1].position = sf::Vector2f(top.getPosition().x + top.getSize().x, top.getPosition().y);
    right[2].position = sf::Vector2f(top.getPosition().x + top.getSize().x, top.getPosition().y + top.getSize().y);
    right[3].position = sf::Vector2f(base.getPosition().x + base.getSize().x, base.getPosition().y + base.getSize().y);
    right[0].color = wallBaseColor; // base point
    right[1].color = wallTopColor;
    right[2].color = wallTopColor;
    right[3].color = wallBaseColor; // base point

    front.setPrimitiveType(sf::Quads);
    front.resize(4);
    front[0].position = sf::Vector2f(top.getPosition().x, top.getPosition().y + top.getSize().y);
    front[1].position = sf::Vector2f(base.getPosition().x, base.getPosition().y + base.getSize().y);
    front[2].position = sf::Vector2f(base.getPosition().x + base.getSize().x, base.getPosition().y + base.getSize().y);
    front[3].position = sf::Vector2f(top.getPosition().x + top.getSize().x, top.getPosition().y + top.getSize().y);
    front[0].color = wallTopColor;
    front[1].color = wallBaseColor; // base point
    front[2].color = wallBaseColor; // base point
    front[3].color = wallTopColor;

    left.setPrimitiveType(sf::Quads);
    left.resize(4);
    left[0].position = sf::Vector2f(top.getPosition());
    left[1].position = sf::Vector2f(base.getPosition());
    left[2].position = sf::Vector2f(base.getPosition().x, base.getPosition().y + base.getSize().y);
    left[3].position = sf::Vector2f(top.getPosition().x, top.getPosition().y + top.getSize().y);
    left[0].color = wallTopColor;
    left[1].color = wallBaseColor; // base point
    left[2].color = wallBaseColor; // base point
    left[3].color = wallTopColor;
}

void Shape3D::draw(sf::RenderWindow &window) {

    float viewStartingSizeX = 960;
    float zoom = viewStartingSizeX/window.getView().getSize().x;

    // arbitrary chosen delta for drawing the bigger the higher the obstacle
    float delta = height*zoom/10;

    // relative position of base rect to view center
    float base1X = base.getPosition().x - window.getView().getCenter().x;
    float base1Y = base.getPosition().y - window.getView().getCenter().y;
    float base2X = base1X + base.getSize().x;
    float base4Y = base1Y + base.getSize().y;

    // top rect position
    float topNew1X = base1X + base1X*delta;
    float topNew1Y = base1Y + base1Y*delta;
    float topNew2X = base2X + base2X*delta;
    float topNew4Y = base4Y + base4Y*delta;

    // bigger size for top rect relating to the position
    float topNewSizeX = topNew2X - topNew1X;
    float topMewSizeY = topNew4Y - topNew1Y;

    // assign new size and new position to the top rect
    top.setSize(sf::Vector2f(topNewSizeX, topMewSizeY));
    top.setPosition(base.getPosition().x + base1X*delta, base.getPosition().y + base1Y*delta);

    transparentRect.setSize(sf::Vector2f(top.getSize().x + 2*outlineThickness, top.getSize().y + 2*outlineThickness));
    transparentRect.setPosition(sf::Vector2f(top.getPosition().x - outlineThickness, top.getPosition().y - outlineThickness));

    // reassign all sides again
    rear[0].position = sf::Vector2f(top.getPosition());
    rear[1].position = sf::Vector2f(top.getPosition().x + top.getSize().x, top.getPosition().y);

    right[1].position = sf::Vector2f(top.getPosition().x + top.getSize().x, top.getPosition().y);
    right[2].position = sf::Vector2f(top.getPosition().x + top.getSize().x, top.getPosition().y + top.getSize().y);

    left[0].position = sf::Vector2f(top.getPosition());
    left[3].position = sf::Vector2f(top.getPosition().x, top.getPosition().y + top.getSize().y);

    front[0].position = sf::Vector2f(top.getPosition().x, top.getPosition().y + top.getSize().y);
    front[3].position = sf::Vector2f(top.getPosition().x + top.getSize().x, top.getPosition().y + top.getSize().y);

    // draw in different order in relation to the view center position
    if (top.getPosition().y <= base.getPosition().y && top.getPosition().y + top.getSize().y >= base.getPosition().y + base.getSize().y) {
        window.draw(left);
        window.draw(right);
        window.draw(top);
        window.draw(transparentRect);
    }

    if (top.getPosition().y < base.getPosition().y && top.getPosition().y + top.getSize().y < base.getPosition().y + base.getSize().y) {
        window.draw(left);
        window.draw(right);
        window.draw(front);
        window.draw(top);
        window.draw(transparentRect);
    }

    if (top.getPosition().y > base.getPosition().y && top.getPosition().y + top.getSize().y > base.getPosition().y + base.getSize().y) {
        window.draw(left);
        window.draw(right);
        window.draw(rear);
        window.draw(top);
        window.draw(transparentRect);
    }

    if (top.getPosition().x > base.getPosition().x) {
        window.draw(left);
        window.draw(top);
    }
}

sf::RectangleShape &Shape3D::getBaseRect() {
    return this->base;
}

sf::Vector2f Shape3D::getSize() {
    return {base.getSize().x/Board::TILE_SIZE, base.getSize().y/Board::TILE_SIZE};
}
