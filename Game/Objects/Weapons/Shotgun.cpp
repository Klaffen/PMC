//
// Created by VegarK on 09.11.2017.
//


#include "Shotgun.h"

#include "../../Board/Board.h"
#include <cmath>
#include <iostream>

shotgun::shotgun() {
    type              = shotgunType;
    name              = getName(type);
    builderCost       = getCost(type);
    damage            = 2;
    maxRange          = 10 * Board::TILE_SIZE;
    distanceReduction = 0.005;
    ammo              = -1;
    apCost            = 2;

    (void) buffer.loadFromFile("Data/Audio/shotgun.wav");
    sound.emplace(buffer);
}

std::vector<sf::RectangleShape> shotgun::Shoot(sf::Vector2f position, sf::Vector2f target) {
    std::vector<sf::RectangleShape> bullets;

    const float deltaX    = position.x - target.x;
    const float deltaY    = position.y - target.y;
    const float baseAngle = (atan2(deltaY, deltaX) + M_PI) * (180.f / M_PI);

    const int numBullets = 5;
    const float spread   = 60.f;
    const float step     = spread / (numBullets - 1);

    const sf::Vector2f shotSize(maxRange, 1);
    for (int i = 0; i < numBullets; i++) {
        const float angle = baseAngle - spread / 2.f + step * i;
        sf::RectangleShape shot(shotSize);
        shot.setPosition(position);
        shot.setRotation(sf::degrees(angle));
        bullets.push_back(shot);
    }
    shapes = bullets;
    drawTime          = 5;
    return bullets;
}
