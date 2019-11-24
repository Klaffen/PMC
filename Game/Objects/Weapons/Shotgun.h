//
// Created by VegarK on 09.11.2017.
//

#ifndef DAT220_PROJECT_SHOTGUN_H
#define DAT220_PROJECT_SHOTGUN_H


#include "WeaponBase.h"
#include "Game/Board/Board.h"

class shotgun : public weaponBase{
public:
    shotgun();

    std::vector<sf::RectangleShape> Shoot(sf::Vector2f position, sf::Vector2f target);

protected:
private:
};


#endif //DAT220_PROJECT_SHOTGUN_H
