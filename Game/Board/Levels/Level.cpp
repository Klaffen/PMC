//
// Created by Roman Aleksandrov on 31/10/2017.
//

#include "Level.h"

#include "../Board.h"
#include <iostream>


Level::Level() {

    // Reserve initial size of obstacle vector (fixes debug issue on windows)
    obstacles.reserve(100);

    Shape3D obstacle1({1, 1}, {27, 10}, 1);
    Shape3D obstacle2({1, 1}, {25, 12}, 1);
    Shape3D obstacle4({1, 3}, {6, 2}, 1);
    Shape3D obstacle6({1, 3}, {13, 4}, 1);
    Shape3D obstacle7({1, 3}, {13, 13}, 1);

    obstacles.push_back(obstacle1);
    obstacles.push_back(obstacle2);
    obstacles.push_back(obstacle4);
    obstacles.push_back(obstacle6);
    obstacles.push_back(obstacle7);

    reflectOverX();
    reflectOverY();
}

void Level::reflectOverY() {

    for (auto& obstacle : obstacles) {

        sf::Vector2f size     = {obstacle.getBaseRect().getSize().x / Board::TILE_SIZE,
                obstacle.getBaseRect().getSize().y / Board::TILE_SIZE};
        sf::Vector2f position = {obstacle.getBaseRect().getPosition().x / Board::TILE_SIZE,
            obstacle.getBaseRect().getPosition().y / Board::TILE_SIZE};

        // 15 is the center tile
        float newPositionX = 15 - position.x - size.x + 15;

        Shape3D newObstacle({size}, {newPositionX, position.y}, 1);
        obstacles.push_back(newObstacle);
    }
}

void Level::reflectOverX() {

    for (auto& obstacle : obstacles) {

        sf::Vector2f size     = {obstacle.getBaseRect().getSize().x / Board::TILE_SIZE,
                obstacle.getBaseRect().getSize().y / Board::TILE_SIZE};
        sf::Vector2f position = {obstacle.getBaseRect().getPosition().x / Board::TILE_SIZE,
            obstacle.getBaseRect().getPosition().y / Board::TILE_SIZE};

        // 8 is the center tile
        float newPositionY = 8 - position.y - size.y + 8;

        Shape3D newObstacle({size}, {position.x, newPositionY}, 1);
        obstacles.push_back(newObstacle);
    }
}

void Level::reflectOverXY() {

    for (auto& obstacle : obstacles) {

        sf::Vector2f size     = {obstacle.getBaseRect().getSize().x / Board::TILE_SIZE,
                obstacle.getBaseRect().getSize().y / Board::TILE_SIZE};
        sf::Vector2f position = {obstacle.getBaseRect().getPosition().x / Board::TILE_SIZE,
            obstacle.getBaseRect().getPosition().y / Board::TILE_SIZE};

        sf::Vector2f newPosition = {15 - position.x - size.x + 15, 8 - position.y - size.y + 8};

        Shape3D newObstacle(size, newPosition, 1);
        obstacles.push_back(newObstacle);
    }
}
