//
// Created by VegarK on 23.10.2017.
//

#ifndef DAT220_PROJECT_RIFLE_H
#define DAT220_PROJECT_RIFLE_H


#include <SFML/Graphics/RectangleShape.hpp>
#include "WeaponBase.h"

class rifle : public weaponBase{
public:
    rifle();
    std::vector<sf::RectangleShape> Shoot(sf::Vector2f position, sf::Vector2f target);
protected:
private:
};


#endif //DAT220_PROJECT_RIFLE_H
