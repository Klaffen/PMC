//
// Created by VegarK on 16.10.2017.
//

#ifndef DAT220_PROJECT_WEAPONBASE_H
#define DAT220_PROJECT_WEAPONBASE_H


#include <memory>
#include <optional>

#include <SFML/Audio.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window.hpp>

class weaponBase {
public:
    virtual ~weaponBase() = default;
    weaponBase() = default;

    sf::String name;
    sf::String getName() const;

    float getDamage(float distance) const;
    double damage; // Base damage at point blank range
    double maxRange; // The furthest the weapon can fire
    double distanceReduction; // Loss of damage based on distance
    int ammo; // How many uses the weapon has
    int apCost; // How many action points a weapon usage costs
    int builderCost; // How much this costs to add in squadbuilder
    virtual std::vector<sf::RectangleShape> Shoot(sf::Vector2f position, sf::Vector2f target);
    enum weaponType { noType = 0, rifleType = 1, shotgunType = 2, grenadeType = 3 };
    static int getCost(weaponType type);
    static sf::String getName(weaponType type);
    weaponType type;
    std::vector<sf::RectangleShape> shapes;

    int drawTime = 0;

    sf::SoundBuffer buffer;
    std::optional<sf::Sound> sound;
    int volume;

private:
protected:
};


#endif // DAT220_PROJECT_WEAPONBASE_H
