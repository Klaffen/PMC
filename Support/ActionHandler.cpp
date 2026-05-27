//
// Created by VegarK on 18.10.2017.
//


#include "ActionHandler.h"

#include "../Game/Board/Board.h"
#include "../Game/GameSettings.h"
#include "../Game/Vision/Vision.h"
#include "../Networking/Network.h"
#include "MathHelper.h"



std::vector<sf::Packet> actionHandler::Move(
    unitBase& unit, float xPos, float yPos, std::vector<std::vector<Tile>>& tileMap) {

    std::vector<sf::Packet> packets;
    sf::Packet packet;
    if (unit.actionPoints >= 1) {
        unit.move(sf::Vector2i(xPos, yPos), tileMap);
        packet << FUNCTION_MOVE << unit.id << unit.player << xPos << yPos;
        packets.push_back(packet);
    }
    return packets;
}

void actionHandler::GetRemoteAction(
    sf::Packet packet, std::vector<unitBase>* unitList, Network* network, Board& gameBoard, MatchState& matchState) {
    int function = -1;

    packet >> function;
    if (function != -1) {
        std::cout << "Packet recieved with function id: " << function << std::endl;
    }

    int playerID;
    int unitID;
    float xPos;
    float yPos;
    int damage;
    int weapon;
    unitBase* unit;
    unitBase::unitClass unitClass;
    switch (function) {
    case FUNCTION_MOVE:
        packet >> unitID >> playerID >> xPos >> yPos;
        unit = getUnit(unitID, playerID, unitList);
        if (unit != nullptr) {
            unit->move(sf::Vector2i(xPos, yPos), gameBoard.tileMap);
        }
        break;

    case FUNCTION_WEAPONSWAP:
        {
            packet >> unitID >> playerID >> weapon;
            unit = getUnit(unitID, playerID, unitList);
            if (unit != nullptr) {
                const auto type = static_cast<weaponBase::weaponType>(weapon);
                weaponSwap(unit, type);
            }
            break;
        }

    case FUNCTION_SHOOT:
        {
            packet >> unitID >> playerID >> xPos >> yPos;
            unit = getUnit(unitID, playerID, unitList);
            if (unit != nullptr) {
                const sf::Vector2f target = sf::Vector2f(xPos, yPos);
                unit->getWeapon()->Shoot(unit->shape.getPosition(), target);
                if (unit->getWeapon()->sound) {
                    unit->getWeapon()->sound->play();
                }
            }
            break;
        }

    case FUNCTION_HURT:
        {
            packet >> unitID >> playerID >> damage;
            std::cout << "Player : " << playerID << "unit : " << unitID << " hurt for " << damage << std::endl;
            unit = getUnit(unitID, playerID, unitList);
            if (unit != nullptr) {
                unit->hurt(damage);
            }
            break;
        }

    case FUNCTION_ADD_UNIT:
        {
            packet >> unitClass.unitId >> unitClass.playerId >> unitClass.health >> unitClass.sightRange
                >> unitClass.actionPoints >> weapon;
            unitClass.weapon = static_cast<weaponBase::weaponType>(weapon);

            std::cout << "Added unit with ID: " << unitClass.unitId << " Player: " << unitClass.playerId
                      << " Health: " << unitClass.health << " Vision: " << unitClass.sightRange
                      << " AP: " << unitClass.actionPoints << " Weapon: " << unitClass.weapon << std::endl;

            unitBase newUnit(matchState.playerNumber, unitClass, gameBoard);
            unitList->push_back(newUnit);
            break;
        }

    case FUNCTION_END_OF_TURN:
        {
            if (!network->isHost()) {
                break;
            }

            int player;
            packet >> player;
            nextTurn(player, *network, unitList, matchState);
            break;
        }

    case FUNCTION_YOUR_TURN:
        {
            matchState.turn        = true;
            matchState.titleString = "Your turn";
            matchState.titleColor  = sf::Color::White;
            for (auto& i : *unitList) {
                i.turn = i.player == matchState.playerNumber;
                i.restoreAP();
            }
            break;
        }

    case FUNCTION_END_OF_GAME:
        {
            int status;
            packet >> status;

            if (status == Client::defeat) {
                matchState.titleString = "You lost!";
                matchState.titleColor  = sf::Color::Red;
            } else if (status == Client::victory) {
                matchState.titleString = "You won!";
                matchState.titleColor  = sf::Color::Green;
            }
            break;
        }

    case REQUEST_MOVE:
        {
            if (!network->isHost()) break;
            packet >> unitID >> playerID >> xPos >> yPos;
            unit = getUnit(unitID, playerID, unitList);
            if (unit != nullptr) {
                auto packets = Move(*unit, xPos, yPos, gameBoard.tileMap);
                for (auto& p : packets) network->sendPacket(p);
            }
            break;
        }

    case REQUEST_SHOOT:
        {
            if (!network->isHost()) break;
            packet >> unitID >> playerID >> xPos >> yPos >> weapon;
            unit = getUnit(unitID, playerID, unitList);
            if (unit != nullptr) {
                const auto type = static_cast<weaponBase::weaponType>(weapon);
                auto packets    = Shoot(*unit, {xPos, yPos}, *unitList, gameBoard, type);
                for (auto& p : packets) network->sendPacket(p);
                victory(*unitList, network, matchState);
            }
            break;
        }

    case REQUEST_WEAPONSWAP:
        {
            if (!network->isHost()) break;
            packet >> unitID >> playerID >> weapon;
            unit = getUnit(unitID, playerID, unitList);
            if (unit != nullptr) {
                const auto type = static_cast<weaponBase::weaponType>(weapon);
                auto packets    = weaponSwap(unit, type);
                for (auto& p : packets) network->sendPacket(p);
            }
            break;
        }

    default:
        break;
    }
}

unitBase* actionHandler::getUnit(int id, int player, std::vector<unitBase>* unitList) {
    for (unitBase& unit : *unitList) {
        if (unit.id == id) {
            if (unit.player == player) {
                return &unit;
            }
        }
    }
    return nullptr;
}


std::vector<sf::Packet> actionHandler::Shoot(
    unitBase& unit, sf::Vector2f target, std::vector<unitBase>& units, Board& board, weaponBase::weaponType type) {
    std::vector<sf::Packet> packetList;
    sf::Packet shootPacket;

    std::shared_ptr<weaponBase> weapon = unit.getWeapon(type);
    if (!weapon) {
        return packetList;
    }

    if (unit.actionPoints >= weapon->apCost) {
        std::vector<sf::RectangleShape> bullets = weapon->Shoot(unit.shape.getPosition(), target);
        weapon->sound->play();
        shootPacket << FUNCTION_SHOOT << unit.id << unit.player << target.x << target.y;
        packetList.push_back(shootPacket);
        unit.spendAP(weapon->apCost);
        for (const auto& bullet : bullets) {
            const Tile unitTile = board.tileMap[(int) (bullet.getPosition().x / Board::TILE_SIZE)].at(
                (int) (bullet.getPosition().y / Board::TILE_SIZE));
            // Loops through all units
            for (auto& testUnit : units) {
                if (ShootingCollision(testUnit.shape, bullet)
                    && !(testUnit.id == unit.id && testUnit.player == unit.player)) {
                    Tile testTile                = board.tileMap[testUnit.position.x].at(testUnit.position.y);
                    std::vector<Tile> vulnerable = Vision::getVisibleTiles(unitTile, weapon->maxRange, board);
                    // Checks if the unit is visible, (not behind a wall) before dealing damage.
                    if (Vision::tileIsInSight(testTile, vulnerable)) {
                        float damage = weapon->getDamage(
                            mathHelper::getDistance(bullet.getPosition(), testUnit.shape.getPosition()));
                        // Check if unit is dead after taking damage
                        testUnit.hurt(damage);
                        sf::Packet newPacket;
                        newPacket << FUNCTION_HURT << testUnit.id << testUnit.player << damage;
                        packetList.push_back(newPacket);
                    }
                }
            }
        }
    }
    return packetList;
}

void actionHandler::victory(std::vector<unitBase>& units, Network* network, MatchState& matchState) {
    std::map<int, bool> playerMap;
    sf::Packet winnerPacket;

    for (auto& unit : units) {
        if (playerMap.find(unit.player) == playerMap.end()
            || (playerMap.find(unit.player) != playerMap.end() && playerMap[unit.player] == false)) {
            playerMap[unit.player] = unit.alive;
        }
    }
    std::cout << "Player and status in playermap:" << std::endl;
    for (const auto& it : playerMap) {
        std::cout << it.first << " " << it.second << std::endl;
    }


    for (int player = 0; player < (int) playerMap.size(); player++) {
        if (player == 0) {
            if (!playerMap[0] && matchState.alive) {
                matchState.alive       = false;
                matchState.titleString = "You lost!";
                matchState.titleColor  = sf::Color::Red;
                playerMap.erase(0);
                std::cout << "Host Defeated!" << std::endl;
                if (matchState.turn) {
                    nextTurn(0, *network, &units, matchState);
                }
            }
        } else {
            std::lock_guard lock(network->clientsMutex);
            if (!playerMap[player] && network->clients.at(player - 1)->status != Client::defeat) {
                network->clients.at(player - 1)->status = Client::defeat;
                network->clients.at(player - 1)->alive  = false;
                winnerPacket << FUNCTION_END_OF_GAME << 3;
                (void) network->clients.at(player - 1)->socket.send(winnerPacket);
                winnerPacket.clear();
                playerMap.erase(player);
                std::cout << "Player : " << player << " Defeated" << std::endl;
            }
        }
    }

    int playersLeft = 0;
    for (const auto& it : playerMap) {
        if (it.second) {
            playersLeft++;
        }
    }

    if (playersLeft > 1) {
        return;
    }

    int winner = -1;

    for (const auto& it : playerMap) {
        if (it.second) {
            winner = it.first;
            break;
        }
    }

    if (winner == -1) return;

    if (winner == matchState.playerNumber) {
        matchState.titleString = "You won!";
        matchState.titleColor  = sf::Color::Green;
    } else {
        matchState.titleString = "You lost!";
        matchState.titleColor  = sf::Color::Red;

        winner--;
        std::lock_guard lock(network->clientsMutex);
        network->clients.at(winner)->status = Client::victory;
        winnerPacket << FUNCTION_END_OF_GAME << Client::victory;
        (void) network->clients.at(winner)->socket.send(winnerPacket);
    }
}

bool actionHandler::ShootingCollision(const sf::CircleShape& unit, const sf::RectangleShape& bullet) {
    const float rotation = bullet.getRotation().asDegrees();
    if (rotation != 0) {
        // Find angle between unit and shooter
        float deltaX = -unit.getPosition().x + bullet.getPosition().x;
        float deltaY = -unit.getPosition().y + bullet.getPosition().y;
        float angle  = (atan2(deltaY, deltaX) + M_PI);
        angle *= (180 / M_PI);

        // Get the full hitbox of the unit, and compare with the bullet rotation
        float distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
        float tangent  = sqrt(pow(distance, 2) + pow(unit.getRadius(), 2));
        float hitBox   = acos(distance / tangent);
        hitBox *= (180 / M_PI);

        //+- vinkel til sirkeltangenten
        if (rotation < angle + hitBox && rotation > angle - hitBox) {
            return true;
        }
        return false;
    }
    return (bullet.getGlobalBounds().findIntersection(unit.getGlobalBounds()).has_value());
}

std::vector<sf::Packet> actionHandler::weaponSwap(unitBase* unit, weaponBase::weaponType type) {
    if (unit->actionPoints > 2) {
        unit->weaponSwap(type);
        unit->spendAP(2);
        sf::Packet packet;
        packet << FUNCTION_WEAPONSWAP << unit->id << unit->player << static_cast<int>(type);
        return std::vector{packet};
    }
    return {};
}

void actionHandler::sendUnit(unitBase unit, ISession* session, unitBase::unitClass unitClass) {
    sf::Packet packet;
    int function = FUNCTION_ADD_UNIT;
    int unitID   = unit.id;
    int playerID = unit.player;
    std::cout << "Sending unit with ID: " << unitID << " Player: " << playerID << " Health: " << unitClass.health
              << " Vision: " << unitClass.sightRange << " AP: " << unitClass.actionPoints
              << " Weapon: " << (int) unitClass.weapon << std::endl;
    packet << function << unitID << playerID << unitClass.health << unitClass.sightRange << unitClass.actionPoints
           << (int) unitClass.weapon;
    session->sendPacket(packet);
}

void actionHandler::nextTurn(int player, Network& network, std::vector<unitBase>* unitList, MatchState& matchState) {
    std::lock_guard lock(network.clientsMutex);
    std::cout << "Input player: " << player << std::endl;
    if (player != 0) {
        network.clients.at(player - 1)->status = network.clients.at(player - 1)->opponentsTurn;
    }

    bool found      = false;
    int checkPlayer = player;

    while (!found) {
        if (checkPlayer + 1 > (int) network.clients.size() && matchState.alive) {
            std::cout << " Next: Host" << std::endl;
            for (auto& i : *unitList) {
                i.turn = true;
                i.restoreAP();
            }
            matchState.turn        = true;
            matchState.titleString = "Your turn";
            matchState.titleColor  = sf::Color::White;
            found                  = true;
        } else if (checkPlayer + 1 > (int) network.clients.size() && !matchState.alive) {
            std::cout << "Skipping hosts turn since he is dead" << std::endl;
            checkPlayer = -1;
        } else if (network.clients.at(checkPlayer)->alive) {
            sf::Packet packet;
            packet << FUNCTION_YOUR_TURN;
            network.clients.at(checkPlayer)->status = network.clients.at(checkPlayer)->turn;
            (void) network.clients.at(checkPlayer)->socket.send(packet);
            found             = true;
            matchState.turn   = false;
            std::cout << " Next: " << checkPlayer << std::endl;
        }
        checkPlayer++;
    }
}
