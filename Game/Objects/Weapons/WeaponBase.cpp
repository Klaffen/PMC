//
// Created by VegarK on 16.10.2017.
//

#include "WeaponBase.h"

#include <cmath>

std::vector<sf::RectangleShape> weaponBase::Shoot(sf::Vector2f position, sf::Vector2f target) {
    return {};
}

int weaponBase::getCost(weaponType type) {
    switch (type) {
    case rifleType:
        return 10;
    case shotgunType:
        return 20;
    case grenadeType:
        return 30;
    default:
        return 0;
    }
}

float weaponBase::getDamage(float distance) const {
    float retDamage;
    retDamage = static_cast<float>((pow(1 - distanceReduction, distance)) * damage);
    return retDamage;
}

sf::String weaponBase::getName() const {
    return name;
}

sf::String weaponBase::getName(weaponType type) {
    switch (type) {
    case rifleType:
        return "Rifle";
    case shotgunType:
        return "Shotgun";
    case grenadeType:
        return "Grenade";
    default:
        return "None";
    }
}
