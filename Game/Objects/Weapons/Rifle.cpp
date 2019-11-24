//
// Created by VegarK on 23.10.2017.
//

#include <cmath>
#include <math.h>
#include <Game/Board/Board.h>
#include <iostream>
#include "Rifle.h"

#define _USE_MATH_DEFINES

rifle::rifle() {
    damage = 40;
    maxRange = 25 * Board::TILE_SIZE;
    distanceReduction = 0.0005;
    ammo = -1;
    apCost = 2;
    type = rifleType;
    builderCost = 0;
    buffer.loadFromFile("Data/Audio/laser.wav");
    sound.setBuffer(buffer);
}

std::vector<sf::RectangleShape> rifle::Shoot(sf::Vector2f position, sf::Vector2f target){
    float deltaX = position.x - target.x;
    float deltaY = position.y - target.y;
    float angle = (atan2(deltaY, deltaX) + M_PI);
    angle *= (180/M_PI);

    std::vector<sf::RectangleShape> shots;
    sf::Vector2f shotSize(maxRange, 1);
    sf::RectangleShape shot = sf::RectangleShape(shotSize);
    shot.setPosition(position);
    //shot.setOrigin(position);
    shot.setSize(shotSize);
    shot.setRotation(angle);
    shot.setFillColor(sf::Color::Red);
    this->shape = shot;
    shots.push_back(shot);
    drawTime = 5;
    return shots;
}



