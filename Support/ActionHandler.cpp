//
// Created by VegarK on 18.10.2017.
//


#include <Game/GameSettings.h>
#include "ActionHandler.h"
#define FUNCTION_MOVE 1001
#define FUNCTION_SHOOT 1002
#define FUNCTION_HURT 1003
#define FUNCTION_WEAPONSWAP 1004
#define FUNCTION_ADDUNIT 1005
#define FUNCTION_END_OF_TURN 1010
#define FUNCTION_YOUR_TURN 1011
#define FUNCTION_END_OF_GAME 1020
#define WEAPON_RIFLE 2010
#define WEAPON_SHOTGUN 2011


std::vector<sf::Packet> actionHandler::actionLog {};

std::vector<sf::Packet> actionHandler::Move(unitBase &unit, float xPos, float yPos, std::vector<std::vector<Tile>> &tileMap) {
    std::vector<sf::Packet> packets;
    sf::Packet packet;
    if (unit.actionPoints >= 1) {
        unit.move(sf::Vector2i(xPos, yPos), tileMap);
        packet << FUNCTION_MOVE << unit.id << unit.player << xPos << yPos;
        packets.push_back(packet);
        for (auto packet : packets)
            actionLog.push_back(packet);
    }
    return packets;
}

void actionHandler::GetRemoteAction(sf::Packet packet, std::vector<unitBase> *unitList, Network *network, Board &gameBoard) {
    int function = -1;

    packet >> function;
    if (function != -1)
        std::cout << "Packet recieved with function id: " << function << std::endl;

    int playerID;
    int unitID;
    float xPos;
    float yPos;
    int damage;
    int weapon;
    sf::Vector2f target;
    unitBase *unit;
    unitBase::unitClass unitClass;
    switch(function) {
        case FUNCTION_MOVE:
            packet >> unitID >> playerID >> xPos >> yPos;
            unit = getUnit(unitID, playerID, unitList);
            if (unit != nullptr) {
                Move(*unit, xPos, yPos, gameBoard.tileMap);
            }
            break;

        case FUNCTION_WEAPONSWAP: {
            packet >> unitID >> playerID >> weapon;
            unit = getUnit(unitID, playerID, unitList);
            if (unit != nullptr) {
                auto type = (weaponBase::weaponType) weapon;
                weaponSwap(unit, type);
            }
            break;
        }

        case FUNCTION_SHOOT: {
            actionLog.push_back(packet);
            packet >> unitID >> playerID >> xPos >> yPos;
            unit = getUnit(unitID, playerID, unitList);
            if (unit != nullptr) {
                target = sf::Vector2f(xPos, yPos);
                unit->getWeapon()->Shoot(unit->shape.getPosition(), target);
                unit->getWeapon()->sound.play();
            }
            break;
        }

        case FUNCTION_HURT: {
            packet >> unitID >> playerID >> damage;
            std::cout << "Player : " << playerID << "unit : " << unitID << " hurt for " << damage << std::endl;
            unit = getUnit(unitID, playerID, unitList);
            if (unit != nullptr) {
                unit->hurt(damage);
                if (network->HOST)
                    victory(*unitList, network);
            }
            break;
        }

        case FUNCTION_ADDUNIT: {
            packet >> unitClass.unitId >> unitClass.playerId >> unitClass.health >> unitClass.sightRange >> unitClass.actionPoints >> weapon;

            if (weapon == 0)
                unitClass.weapon = weaponBase::noType;
            else if (weapon == 1)
                unitClass.weapon = weaponBase::rifleType;
            else if (weapon == 2)
                unitClass.weapon = weaponBase::shotgunType;
            else if (weapon == 3)
                unitClass.weapon = weaponBase::grenadeType;

            std::cout << "Added unit with ID: " << unitClass.unitId << " Player: " << unitClass.playerId << " Health: " << unitClass.health << " Vision: " << unitClass.sightRange <<
                      " AP: " << unitClass.actionPoints << " Weapon: " << unitClass.weapon << std::endl;

            unitBase newUnit(network, unitClass, gameBoard);
            unitList->push_back(newUnit);
            break;
        }

        case FUNCTION_END_OF_TURN: {
            if (!network->HOST)
                break;

            int player;
            packet >> player;
            nextTurn(player, *network,unitList);
            break;
        }

        case FUNCTION_YOUR_TURN: {
            network->turn = true;
            network->title.setString("Your turn");
            network->title.setFillColor(sf::Color::White);
            for (auto &i : *unitList) {
                i.turn = i.player == network->playerNumber;
                i.restoreAP();
            }
            break;
        }

        case FUNCTION_END_OF_GAME: {
            int status;
            packet >> status;

            if (status == 3) {
                network->title.setString("You lost!");
                network->title.setFillColor(sf::Color::Red);
            }
            else if (status == 4) {
                network->title.setString("You won!");
                network->title.setFillColor(sf::Color::Green);
            }
            break;
        }

        default:
            break;

    }
}

unitBase *actionHandler::getUnit(int id, int player, std::vector<unitBase> *unitList) {
    for (unitBase & unit : *unitList) {
        if (unit.id == id)
            if (unit.player == player) {
                return &unit;
            }
    }
    return nullptr;
}


std::vector<sf::Packet> actionHandler::Shoot(unitBase &unit, sf::Vector2f target, std::vector<unitBase> &units, Board &board) {
    std::vector<sf::Packet> packetList;
    sf::Packet shootPacket;

    if(unit.actionPoints >= unit.getWeapon()->apCost) {
        std::vector<sf::RectangleShape> bullets = unit.getWeapon()->Shoot(unit.shape.getPosition(), target);
        GameSettings::playUnitSound(unit);
        shootPacket << FUNCTION_SHOOT << unit.id << unit.player << target.x << target.y;
        packetList.push_back(shootPacket);
        unit.spendAP(unit.getWeapon()->apCost);
        //Loops through each 'bullet' fired from the weapon
        for (auto bullet : bullets) {
            Tile unitTile = board.tileMap[(int) (bullet.getPosition().x / Board::TILE_SIZE)].at((int) (bullet.getPosition().y / Board::TILE_SIZE));
            //Loops through all units
            for (auto &testUnit : units) {
                if (ShootingCollision(testUnit.shape, bullet) &&
                    !(testUnit.id == unit.id && testUnit.player == unit.player)) {
                    Tile testTile = board.tileMap[testUnit.position.x].at(testUnit.position.y);
                    std::vector<Tile> vulnerable = Vision::getVisibleTiles(unitTile, unit.getWeapon()->maxRange, board);
                    //Checks if the unit is visible, (not behind a wall) before dealing damage.
                    if (Vision::tileIsInSight(testTile, vulnerable)) {
                        float damage = unit.getWeapon()->getDamage(
                                mathHelper::getDistance(bullet.getPosition(), testUnit.shape.getPosition()));
                        //Check if unit is dead after taking damage
                        testUnit.hurt(damage);
                        sf::Packet newPacket;
                        newPacket << FUNCTION_HURT << testUnit.id << testUnit.player << damage;
                        packetList.push_back(newPacket);
                    }
                }
            }
        }
    for (auto packet : packetList)
        actionLog.push_back(packet);
    }
    return packetList;
}

void actionHandler::victory(std::vector<unitBase> &units, Network *network) {
    std::map<int, bool> playerMap;
    sf::Packet winnerPacket;

    for (auto &unit : units) {
        if (playerMap.find(unit.player) == playerMap.end() || (playerMap.find(unit.player) != playerMap.end() && playerMap[unit.player] == false)) {
            playerMap[unit.player] = unit.alive;
        }
    }
    std::cout << "Player and status in playermap:" << std::endl;
    for (const auto &it : playerMap)
        std::cout << it.first << " " << it.second << std::endl;


    for (int player = 0; player < (int) playerMap.size(); player++) {
        if (player == 0) {
            if (!playerMap[0] && network->alive) {
                network->alive = false;
                network->title.setString("You lost!");
                network->title.setFillColor(sf::Color::Red);
                playerMap.erase(0);
                std::cout << "Host Defeated!" << std::endl;
                if (network->turn)
                    nextTurn(0, *network, &units);
            }
        }
        else {
            if (!playerMap[player] && network->clients.at(player - 1)->status != network->clients.at(player - 1)->defeat) {
                network->clients.at(player - 1)->status = network->clients.at(player - 1)->defeat;
                network->clients.at(player - 1)->alive = false;
                winnerPacket << FUNCTION_END_OF_GAME << 3;
                network->clients.at(player - 1)->socket.send(winnerPacket);
                winnerPacket.clear();
                playerMap.erase(player);
                std::cout << "Player : " << player << " Defeated" << std::endl;
            }
        }
    }

    int playersLeft = 0;
    for (const auto &it : playerMap)
        if (it.second)
            playersLeft++;

    if (playersLeft > 1)
        return;

    int winner;

    for (const auto &it : playerMap)
        if (it.second) {
            winner = it.first;
            break;
        }

    if (winner == network->playerNumber) {
        network->title.setString("You won!");
        network->title.setFillColor(sf::Color::Green);
        return;
    }
    else {
        network->title.setString("You lost!");
        network->title.setFillColor(sf::Color::Red);

        winner--;
        network->clients.at(winner)->status = network->clients.at(winner)->victory;
        winnerPacket << FUNCTION_END_OF_GAME << 4;
        network->clients.at(winner)->socket.send(winnerPacket);
    }
}

bool actionHandler::ShootingCollision(sf::CircleShape unit, sf::RectangleShape bullet) {
    float rotation = bullet.getRotation();
    if (rotation != 0){
        //Find angle between unit and shooter
        float deltaX = -unit.getPosition().x + bullet.getPosition().x;
        float deltaY = -unit.getPosition().y + bullet.getPosition().y;
        float angle = (atan2(deltaY, deltaX) + M_PI);
        angle *= (180/M_PI);

        //Get the full hitbox of the unit, and compare with the bullet rotation
        float distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
        float tangent = sqrt(pow(distance, 2) + pow(unit.getRadius(), 2));
        float hitBox = acos(distance/tangent);
        hitBox *= (180/M_PI);

        //+- vinkel til sirkeltangenten
        if (rotation < angle + hitBox && rotation > angle - hitBox)
            return true;
        return false;
    }
    else
        return (bullet.getGlobalBounds().intersects(unit.getGlobalBounds()));
}

std::vector<sf::Packet>actionHandler::weaponSwap(unitBase *unit, weaponBase::weaponType type) {
    if(unit->actionPoints > 2) {
        unit->weaponSwap(type);
        unit->spendAP(2);
    }
    int sendType = (int(type));
    sf::Packet packet;
    packet << FUNCTION_WEAPONSWAP << unit->id << unit->player << sendType;
    actionLog.push_back(packet);
    return std::vector<sf::Packet>{packet};
}

void actionHandler::sendUnit(unitBase unit, Network *network, unitBase::unitClass unitClass) {
    sf::Packet packet;
    int function = FUNCTION_ADDUNIT;
    int unitID = unit.id;
    int playerID = unit.player;
    std::cout << "Sending unit with ID: " << unitID << " Player: " << playerID << " Health: " << unitClass.health << " Vision: " << unitClass.sightRange <<
              " AP: " << unitClass.actionPoints << " Weapon: " << (int) unitClass.weapon << std::endl;
    packet << function << unitID << playerID << unitClass.health << unitClass.sightRange << unitClass.actionPoints << (int) unitClass.weapon;
    network->sendPacket(packet, false);
}

void actionHandler::nextTurn(int player, Network &network, std::vector<unitBase> *unitList) {
    std::cout << "Input player: " << player << std::endl;
    if (player != 0)
        network.clients.at(player - 1)->status = network.clients.at(player - 1)->opponentsTurn;

    bool found = false;
    int checkPlayer = player;

    while (!found) {
        if (checkPlayer + 1 > (int) network.clients.size() && network.alive) {
            std::cout << " Next: Host" << std::endl;
            for (auto &i : *unitList) {
                i.turn = true;
                i.restoreAP();
            }
            network.turn = true;
            network.title.setString("Your turn");
            network.title.setFillColor(sf::Color::White);
            found = true;
        }
        else if (checkPlayer + 1 > (int) network.clients.size() && !network.alive) {
            std::cout << "Skipping hosts turn since he is dead" << std::endl;
            checkPlayer = -1;
        }
        else if (network.clients.at(checkPlayer)->alive) {
            sf::Packet packet;
            packet << FUNCTION_YOUR_TURN;
            network.clients.at(checkPlayer)->status = network.clients.at(checkPlayer)->turn;
            network.clients.at(checkPlayer)->socket.send(packet);
            found = true;
            network.turn = false;
            std::cout << " Next: " << checkPlayer << std::endl;
        }
        checkPlayer++;
    }
}

