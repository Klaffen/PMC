//
// Created by VegarK on 26.10.2017.
//

#include <Game/Board/Board.h>
#include "Grenade.h"

grenade::grenade() {
    type = grenadeType;
    damage = 20;
    maxRange = 200 * Board::TILE_SIZE;
    distanceReduction = 50;
    ammo = -1;
    apCost = 4;
    builderCost = 0;
    area = 4;
    buffer.loadFromFile("Data/Audio/grenade.wav");
    sound.setBuffer(buffer);
}

std::vector<sf::RectangleShape> grenade::Shoot(sf::Vector2f position, sf::Vector2f target) {
    std::vector<sf::RectangleShape> explosions;
    for (int i = 1; i <= 4; i++) {
        sf::Vector2f explosionSize(i * Board::TILE_SIZE,i * Board::TILE_SIZE);
        sf::RectangleShape bang(explosionSize);
        bang.setOrigin(explosionSize.x/2, explosionSize.y/2);
        bang.setPosition(target);
        shape = bang;
        explosions.push_back(bang);
        drawTime = 10;
    }
    return explosions;
}



