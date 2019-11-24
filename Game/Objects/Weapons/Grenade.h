//
// Created by VegarK on 26.10.2017.
//

#ifndef DAT220_PROJECT_GRENADE_H
#define DAT220_PROJECT_GRENADE_H


#include <SFML/Graphics/RectangleShape.hpp>
#include "WeaponBase.h"

class grenade : public weaponBase{
public:
    int area;
    grenade();
    std::vector<sf::RectangleShape> Shoot(sf::Vector2f position, sf::Vector2f target);
};


#endif //DAT220_PROJECT_GRENADE_H
