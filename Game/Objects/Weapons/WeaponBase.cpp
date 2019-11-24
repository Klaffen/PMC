//
// Created by VegarK on 16.10.2017.
//

#include <cmath>
#include "WeaponBase.h"

std::vector<sf::RectangleShape> weaponBase::Shoot(sf::Vector2f position, sf::Vector2f target) {
    return std::vector<sf::RectangleShape>();
}

float weaponBase::getDamage(float distance) {
    float retDamage;
    retDamage = static_cast<float>((pow(1-distanceReduction, distance)) * damage);
    return retDamage;
}
