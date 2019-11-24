//
// Created by VegarK on 16.10.2017.
//

#ifndef DAT220_PROJECT_WEAPONBASE_H
#define DAT220_PROJECT_WEAPONBASE_H


#include <memory>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Audio.hpp>

class weaponBase {
public:
    weaponBase() = default;

    float getDamage(float distance);
    double damage;      //Base damage at point blank range
    double maxRange;    //The furthest the weapon can fire
    double distanceReduction; //Loss of damage based on distance
    int ammo;           //How many uses the weapon has
    int apCost;         //How many action points a weapon usage costs
    int builderCost;    //How much this costs to add in squadbuilder
    virtual std::vector<sf::RectangleShape> Shoot(sf::Vector2f position, sf::Vector2f target);
    enum weaponType{noType, rifleType, shotgunType, grenadeType};
    weaponType type;
    sf::RectangleShape shape;

    int drawTime;

    sf::Sound sound;
    sf::SoundBuffer buffer;
    int volume;
private:
protected:
};


#endif //DAT220_PROJECT_WEAPONBASE_H
