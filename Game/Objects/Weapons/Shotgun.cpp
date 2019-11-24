//
// Created by VegarK on 09.11.2017.
//


#include <iostream>
#include "Shotgun.h"

shotgun::shotgun() {
    type = grenadeType;
    damage = 10;
    maxRange = 10 * Board::TILE_SIZE;
    distanceReduction = 0.005;
    ammo = -1;
    apCost = 2;
    builderCost = 0;

    buffer.loadFromFile("Data/Audio/shotgun.wav");
    sound.setBuffer(buffer);
}

std::vector<sf::RectangleShape> shotgun::Shoot(sf::Vector2f position, sf::Vector2f target) {
    std::vector<sf::RectangleShape> bullets;

    float deltaX = position.x - target.x;
    float deltaY = position.y - target.y;
    float angle = (atan2(deltaY, deltaX) + M_PI);
    angle *= (180/M_PI);
    sf::Vector2f shotSize(maxRange, 1);
    sf::RectangleShape shot = sf::RectangleShape(shotSize);
    for (int i = 0; i < 20; i++){
        shot.setPosition(position);
        shot.setSize(shotSize);
        float randomAngle = (rand() % 22 - 11 + angle);
        std::cout << "angle. " << angle << " randomAngle: " << randomAngle << std::endl;
        shot.setRotation(randomAngle);
        bullets.push_back(shot);
    }

    return bullets;
}

