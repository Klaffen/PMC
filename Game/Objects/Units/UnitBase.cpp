//
// Created by VegarK on 16.10.2017.
//

#include "UnitBase.h"

#include "../../Board/Board.h"
#include "../../Pathfinding/Pathfinding.h"
#include "../../Vision/Vision.h"
#include "../Weapons/Grenade.h"
#include "../Weapons/Rifle.h"
#include "../Weapons/Shotgun.h"

unitBase::unitBase(int localPlayerNumber, unitClass unitClass, Board& gameBoard) {
    id     = unitClass.unitId;
    player = unitClass.playerId; // Get current player number
    if (player == 0) {
        position.x = 0;
        position.y = id * 2 + 5;
    } else if (player == 1) {
        position.x = gameBoard.tileMap.size() - 1;
        position.y = gameBoard.tileMap.at(0).size() - id * 2 - 5;
    } else if (player == 2) {
        position.x = gameBoard.tileMap.size() - 1;
        position.y = id * 2 + 5;
    } else if (player == 3) {
        position.x = 0;
        position.y = gameBoard.tileMap.at(0).size() - id * 2 - 5;
    }

    nextTile      = position;
    currentWeapon = 0;
    weapons       = std::vector<std::shared_ptr<weaponBase>>{};

    health     = unitClass.health;
    maxHealth  = health;
    sightRange = unitClass.sightRange;
    maxAP      = unitClass.actionPoints;
    switch (unitClass.weapon) {
    case weaponBase::rifleType:
        weapons.push_back(std::make_shared<rifle>());
        break;

    case weaponBase::shotgunType:
        weapons.push_back(std::make_shared<shotgun>());
        break;

    case weaponBase::grenadeType:
        weapons.push_back(std::make_shared<grenade>());
        break;

    case weaponBase::noType:
        break;
    }

    if (unitClass.weapon != weaponBase::grenadeType) {
        weapons.push_back(std::make_shared<grenade>());
    }

    Tile& startTile   = gameBoard.tileMap.at(position.x).at(position.y);
    startTile.hasUnit = true;
    if (player == localPlayerNumber) {
        visibleTiles = Vision::getVisibleTiles(startTile, sightRange, gameBoard);
        for (auto& tile : visibleTiles) {
            gameBoard.tileMap.at(Pathfinding::getTileIndex(tile).x).at(Pathfinding::getTileIndex(tile).y).isVisible++;
        }
    }

    actionPoints = maxAP;
    shape.setRadius(Board::TILE_SIZE / 5 * 2);
    shape.setOutlineColor(sf::Color::White);
    if (player == 0) {
        turn  = true;
        color = sf::Color::Green;
    } else if (player == 1) {
        color = sf::Color::Blue;
        turn  = false;
    } else if (player == 2) {
        color = sf::Color::Red;
        turn  = false;
    } else if (player == 3) {
        color = sf::Color::Cyan;
        turn  = false;
    }
    shape.setFillColor(color);
    shape.setOrigin({shape.getRadius(), shape.getRadius()});
    shape.setPosition(
        {position.x * Board::TILE_SIZE + Board::TILE_SIZE / 2, position.y * Board::TILE_SIZE + Board::TILE_SIZE / 2});
    this->localPlayerNumber = localPlayerNumber;
}


void unitBase::process(Board& gameBoard, sf::RenderWindow& window, std::vector<unitBase>& units) {

    if (hasMoved && !isMoving) {
        movementMap = Pathfinding::getMovementMap(
            gameBoard.tileMap.at(position.x).at(position.y), actionPoints, gameBoard.tileMap);
        hasMoved = false;
    }

    if (!path.empty() || position != nextTile) {
        sf::Vector2f goal = sf::Vector2f(nextTile.x * Board::TILE_SIZE + (Board::TILE_SIZE / 2),
            nextTile.y * Board::TILE_SIZE + (Board::TILE_SIZE / 2));
        if (shape.getPosition().x < goal.x + 1 && shape.getPosition().x > goal.x - 1
            && shape.getPosition().y < goal.y + 1 && shape.getPosition().y > goal.y - 1) {

            shape.setPosition(goal);
            position = nextTile;
            if (!path.empty()) {
                spendAP(1); // Spend one AP per tile moved
                hasMoved                                                = true;
                isMoving                                                = true;
                gameBoard.tileMap.at(nextTile.x).at(nextTile.y).hasUnit = false;
                nextTile                                                = path.front();
                path.pop();
                gameBoard.tileMap.at(nextTile.x).at(nextTile.y).hasUnit = true;

                if (player == localPlayerNumber) {
                    updateVision(gameBoard);
                }
            }
        }
        Animate(goal);
    } else {
        isMoving = false;
    }


    visible = gameBoard.tileMap.at(position.x).at(position.y).isVisible != 0;
}

void unitBase::Draw(sf::RenderWindow& window) {
    if (player == localPlayerNumber || visible) {
        window.draw(shape);
    }
    for (auto& weapon : weapons) {
        if (weapon->drawTime != 0) {
            for (const auto& s : weapon->shapes) {
                window.draw(s);
            }
            weapon->drawTime--;
        }
    }
}

void unitBase::hurt(int damage) {
    health -= damage;
    if (health <= 0 && alive) {
        health = 0;
        alive  = false;
        shape.setFillColor(sf::Color(color.r, color.g, color.b, 255 * 0.10));
    }
}

void unitBase::setColor(sf::Color colour) {
    auto healthPercent = health / maxHealth;

    if (healthPercent < 15) {
        healthPercent = 15;
    }
    shape.setFillColor(sf::Color(color.r, color.g, color.b, 255 * healthPercent));
}

void unitBase::shoot(sf::RenderWindow& window, sf::Vector2f target) {
    /*Get firing angle
     *make flat rectangle in firing angle
     *Check for intersection with object
     * check which intersect is closest
     * if ([intersected object] is a unit)
     *      [intersected unit].hurt
     * else if ([intersected object] is a wall)
     *      Damage the wall based on weapon damage
     */
}

void unitBase::move(sf::Vector2i target, std::vector<std::vector<Tile>>& tileMap) {
    Tile& startTile   = tileMap.at(position.x).at(position.y);
    Tile targetTile   = tileMap.at(target.x).at(target.y);
    startTile.hasUnit = false;
    std::vector<sf::Vector2i> pathVector =
        Pathfinding::getPath(startTile, targetTile, movementMap, actionPoints, tileMap);
    for (auto tile : pathVector) {
        path.push(tile);
    }
    if (path.empty()) {
        return;
    }
    path.pop();
    nextTile = path.front();
}

void unitBase::Animate(sf::Vector2f goal) {
    sf::Vector2f newPos = shape.getPosition();
    if (shape.getPosition().x + 1 < goal.x) {
        newPos.x += 2;
    } else if (shape.getPosition().x > goal.x) {
        newPos.x -= 2;
    }

    if (shape.getPosition().y + 1 < goal.y) {
        newPos.y += 2;
    } else if (shape.getPosition().y > goal.y) {
        newPos.y -= 2;
    }

    shape.setPosition(newPos);
}

void unitBase::weaponSwap(weaponBase::weaponType type) {
    for (int i = 0; i < (int) weapons.size(); i++) {
        if (weapons[i]->type == type) {
            currentWeapon = i;
        }
    }
}

std::shared_ptr<weaponBase> unitBase::getWeapon() {
    return weapons[currentWeapon];
}

std::shared_ptr<weaponBase> unitBase::getWeapon(weaponBase::weaponType type) {
    for (auto& weapon : weapons) {
        if (weapon->type == type) {
            return weapon;
        }
    }
    return nullptr;
}

void unitBase::spendAP(int points) {
    actionPoints -= points;
}

void unitBase::restoreAP() {
    actionPoints = std::min(actionPoints + apPerTurn, maxAP);
    hasMoved     = true;
}

unitBase::unitBase() {}

void unitBase::updateVision(Board& board) {
    for (auto& tile : visibleTiles) {
        board.tileMap.at(Pathfinding::getTileIndex(tile).x).at(Pathfinding::getTileIndex(tile).y).isVisible--;
    }

    visibleTiles = Vision::getVisibleTiles(board.tileMap.at(position.x).at(position.y), sightRange, board);
    for (auto& tile : visibleTiles) {
        board.tileMap.at(Pathfinding::getTileIndex(tile).x).at(Pathfinding::getTileIndex(tile).y).isVisible++;
    }
}
