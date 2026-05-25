//
// Created by VegarK on 23.10.2017.
//

#define _USE_MATH_DEFINES

#include <cmath>
#include "../../Board/Board.h"
#include "Rifle.h"

rifle::rifle() {
    damage = 40;
    maxRange = 25 * Board::TILE_SIZE;
    distanceReduction = 0.0005;
    ammo = -1;
    apCost = 2;
    type = rifleType;
    builderCost = 0;
    (void)buffer.loadFromFile("Data/Audio/laser.wav");
    sound.emplace(buffer);
}

std::vector<sf::RectangleShape> rifle::Shoot(sf::Vector2f position, sf::Vector2f target){
    const float deltaX = position.x - target.x;
    const float deltaY = position.y - target.y;
    float angle = (atan2(deltaY, deltaX) + M_PI);
    angle *= (180/M_PI);

    std::vector<sf::RectangleShape> shots;
    const sf::Vector2f shotSize(maxRange, 1);
    sf::RectangleShape shot = sf::RectangleShape(shotSize);
    shot.setPosition(position);
    //shot.setOrigin(position);
    shot.setRotation(sf::degrees(angle));
    shot.setFillColor(sf::Color::Red);
    this->shape = shot;
    shots.push_back(shot);
    drawTime = 5;
    return shots;
}



