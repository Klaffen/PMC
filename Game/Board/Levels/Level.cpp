//
// Created by Roman Aleksandrov on 31/10/2017.
//

#include <iostream>
#include "Level.h"
#include <Game/Board/Board.h>


Level::Level() {

    //Reserve initial size of obstacle vector (fixes debug issue on windows)
    obstacles.reserve(100);

    Shape3D obstacle1({1,1}, {27,10}, 1);
    Shape3D obstacle2({1,1}, {25,12}, 1);
    Shape3D obstacle3({1,1}, {23,14}, 1);
    Shape3D obstacle4({1,5}, {6,2}, 1);
    Shape3D obstacle5({1,5}, {6,10}, 1);
    Shape3D obstacle6({1,4}, {13,4}, 1);
    Shape3D obstacle7({1,3}, {13,13}, 1);
    Shape3D obstacle8({1,1}, {16,13}, 1);
    Shape3D obstacle9({1,1}, {18,11}, 1);
    Shape3D obstacle10({1,1}, {20,9}, 1);
    Shape3D obstacle11({1,1}, {22,7}, 1);
    Shape3D obstacle12({1,1}, {24,5}, 1);
    Shape3D obstacle13({1,1}, {26,3}, 1);
    Shape3D obstacle14({1,1}, {28,1}, 1);

    obstacles.push_back(obstacle1);
    obstacles.push_back(obstacle2);
    obstacles.push_back(obstacle3);
    obstacles.push_back(obstacle4);
    obstacles.push_back(obstacle5);
    obstacles.push_back(obstacle6);
    obstacles.push_back(obstacle7);
    obstacles.push_back(obstacle8);
    obstacles.push_back(obstacle9);
    obstacles.push_back(obstacle10);
    obstacles.push_back(obstacle11);
    obstacles.push_back(obstacle12);
    obstacles.push_back(obstacle13);
    obstacles.push_back(obstacle14);

    reflectOverX();
    reflectOverY();
}

void Level::reflectOverY() {

    for (auto &obstacle : obstacles) {

        sf::Vector2f size = {obstacle.getBaseRect().getSize().x/Board::TILE_SIZE, obstacle.getBaseRect().getSize().y/Board::TILE_SIZE};
        sf::Vector2f position = {obstacle.getBaseRect().getPosition().x/Board::TILE_SIZE, obstacle.getBaseRect().getPosition().y/Board::TILE_SIZE};

        // 30 is the center tile
        float newPositionX = 30 - position.x - size.x + 30;

        Shape3D newObstacle({size}, {newPositionX, position.y}, 1);
        obstacles.push_back(newObstacle);
    }
}

void Level::reflectOverX() {

    for (auto &obstacle : obstacles) {

        sf::Vector2f size = {obstacle.getBaseRect().getSize().x/Board::TILE_SIZE, obstacle.getBaseRect().getSize().y/Board::TILE_SIZE};
        sf::Vector2f position = {obstacle.getBaseRect().getPosition().x/Board::TILE_SIZE, obstacle.getBaseRect().getPosition().y/Board::TILE_SIZE};

        // 17 is the center tile
        float newPositionY = 17 - position.y - size.y + 17;

        Shape3D newObstacle({size}, {position.x, newPositionY}, 1);
        obstacles.push_back(newObstacle);
    }
}

void Level::reflectOverXY() {

    for (auto &obstacle : obstacles) {

        sf::Vector2f size = {obstacle.getBaseRect().getSize().x/Board::TILE_SIZE, obstacle.getBaseRect().getSize().y/Board::TILE_SIZE};
        sf::Vector2f position = {obstacle.getBaseRect().getPosition().x/Board::TILE_SIZE, obstacle.getBaseRect().getPosition().y/Board::TILE_SIZE};

        sf::Vector2f newPosition = {30 - position.x - size.x + 30, 17 - position.y - size.y + 17};

        Shape3D newObstacle(size, newPosition, 1);
        obstacles.push_back(newObstacle);
    }
}
