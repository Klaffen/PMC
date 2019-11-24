//
// Created by VegarK on 16.10.2017.
//

#ifndef DAT220_PROJECT_UNITBASE_H
#define DAT220_PROJECT_UNITBASE_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Network/Packet.hpp>
#include "Networking/Network.h"
#include "Game/Board/Board.h"
#include "Game/Objects/Weapons/Rifle.h"
#include "Game/Objects/Weapons/Grenade.h"
#include "Game/Objects/Weapons/Shotgun.h"
#include "Game/Input.h"
#include "Game/Vision/Vision.h"
#include <Networking/Network.h>
#include <queue>
#include "Game/Pathfinding/Pathfinding.h"
#include "Game/Objects/Weapons/WeaponBase.h"
#include "Game/Board/Board.h"



class unitBase {
public:
    unitBase();
    struct unitClass {
        weaponBase::weaponType weapon = weaponBase::weaponType::noType;
        int sightRange = 1;
        int health = 15;
        int maxHealth = 15;
        int actionPoints = 1;
        int builderCost = 0;

        int unitId;
        int playerId;
    };

    unitBase(Network *network, unitClass unitClass, Board &gameBoard);
    int currentWeapon;      //What weapon the unit currently uses
    int health;          //Unit current health
    int maxHealth;
    int sightRange;
    sf::Vector2i position;  //Unit position on the map
    bool alive = true;            //Is the unit dead?
    bool turn = false;
    sf::CircleShape shape;  //The shape to be drawn
    void spendAP(int points);
    void restoreAP();
    void shoot(sf::RenderWindow &window, sf::Vector2f target);
    void move(sf::Vector2i target, std::vector<std::vector<Tile>> &tileMap);
    void hurt(int damage);
    void process(Board &gameBoard, sf::RenderWindow &window, std::vector<unitBase> &units);
    std::vector<Tile> visibleTiles{};
    void weaponSwap(weaponBase::weaponType type);
    std::shared_ptr<weaponBase> getWeapon();
    virtual void Draw(sf::RenderWindow &window);
    int id;
    int player;
    int actionPoints;
    std::vector<std::shared_ptr<weaponBase>> weapons;
    bool visible;
    sf::Color color;
    void setColor(sf::Color colour);
    std::queue<sf::Vector2i> path;
    std::map<int, int> movementMap;
    bool isMoving = false;

private:
    bool hasMoved = true;

protected:
    void updateVision(Board& board);
    Network *network;
    int maxAP;
    sf::Vector2i nextTile;
    void Animate(sf::Vector2f goal);

};


#endif //DAT220_PROJECT_UNITBASE_H
