//
// Created by VegarK on 13.11.2017.
//

#include "MathHelper.h"

float mathHelper::getAngle(sf::Vector2f p1, sf::Vector2f p2) {
    float deltaX = -p1.x + p2.x;
    float deltaY = -p1.y + p2.y;
    float angle = (atan2(deltaY, deltaX) + M_PI);
    angle *= (180/M_PI);
    return angle;
}

float mathHelper::getDistance(sf::Vector2f p1, sf::Vector2f p2) {
    float deltaX = -p1.x + p2.x;
    float deltaY = -p1.y + p2.y;
    return sqrt(pow(deltaX, 2) + pow(deltaY, 2));
    ;
}
