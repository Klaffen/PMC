//
// Created by Roman Aleksandrov on 18/10/2017.
//


#include <StateMachine/Game.h>
#include "Board.h"

const float Board::TILE_SIZE = 32;

void Board::init(sf::RenderWindow &window) {

    mapSize = sf::Vector2i(1920, 1080);

    for (int i = 0; i < mapSize.x; i += (int) TILE_SIZE) {
        std::vector<Tile> tiles;
        for (int j = 0; j < mapSize.y; j += (int) TILE_SIZE) {
            Tile tile(sf::Vector2f(i, j), (int) TILE_SIZE);
            tiles.push_back(tile);
        }
        tileMap.push_back(tiles);
    }

    setTileTaken(level.obstacles, tileMap);
}

void Board::drawTiles(sf::RenderWindow &window) {
    for (auto &tileList : tileMap){
        for (Tile &tile: tileList) {
            if (tile.isVisible)
                window.draw(tile);
        }
    }
}

void Board::drawObstacles(sf::RenderWindow &window) {
    for (auto &obstacle: level.obstacles) {

        for (int i = 0; i < obstacle.getSize().x; ++i) {
            for (int j = 0; j < obstacle.getSize().y; ++j) {
                if (tileMap[getTileCoord(obstacle.getBaseRect().getPosition()).x + i][getTileCoord(obstacle.getBaseRect().getPosition()).y + j].isVisible){
                    obstacle.draw(window);
                    break;
                }
            }
        }
    }
}

//todo: why do we need view here?
void Board::moveUp(sf::RenderWindow &window, sf::View &view) {
    view.move(Game::viewMoveStep * sin(view.getRotation() * M_PI / 180), -Game::viewMoveStep * cos(view.getRotation() * M_PI / 180));
    window.setView(view);

    // reset view if it goes outside upper window bound
    if (view.getCenter().y < view.getSize().y/10) {
    view.setCenter(view.getCenter().x, view.getSize().y/10);
    window.setView(view);
    }
}

void Board::moveDown(sf::RenderWindow &window, sf::View &view) {
    view.move(-Game::viewMoveStep * sin(view.getRotation() * M_PI / 180.0f), Game::viewMoveStep * cos(view.getRotation() * M_PI / 180.0f));
    window.setView(view);


    // reset view if it goes outside lower window bound
    if (view.getCenter().y > window.getSize().y - view.getSize().y/10) {
    view.setCenter(view.getCenter().x, window.getSize().y - view.getSize().y/10);
    window.setView(view);
    }
}

void Board::moveLeft(sf::RenderWindow &window, sf::View &view) {
    view.move(-Game::viewMoveStep * cos(view.getRotation() * M_PI / 180), -Game::viewMoveStep * sin(view.getRotation() * M_PI / 180));
    window.setView(view);


    // reset view if it goes outside left window bound
    if (view.getCenter().x < view.getSize().x/4) {
    view.setCenter(view.getSize().x/4, view.getCenter().y);
    window.setView(view);
    }
}

void Board::moveRight(sf::RenderWindow &window, sf::View &view) {
    view.move(Game::viewMoveStep * cos(view.getRotation() * M_PI / 180), Game::viewMoveStep * sin(view.getRotation() * M_PI / 180));
    window.setView(view);


    // reset view if it goes outside right window bound
    if (view.getCenter().x > window.getSize().x - view.getSize().x/4) {
    view.setCenter(window.getSize().x - view.getSize().x/4, view.getCenter().y);
    window.setView(view);
    }
}

void Board::Process(sf::RenderWindow &window, sf::View &view) {

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        moveUp(window, view);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        moveDown(window, view);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        moveLeft(window, view);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        moveRight(window, view);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        view.rotate(-0.9f);
        window.setView(view);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        view.rotate(0.9f);
        window.setView(view);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
        // zoom will not be always > minZoom, very small deviation
        if (Game::viewStartingSize.x / view.getSize().x > Game::minZoom) {
            view.zoom(1.01);
            window.setView(view);
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        // zoom will not be always < maxZoom very small deviation
        if (Game::viewStartingSize.x / view.getSize().x < Game::maxZoom) {
            view.zoom(0.99);
            window.setView(view);
        }
    }
}

sf::Vector2i Board::getTileCoord(sf::Vector2f position) {
    int tileX = (int)roundf(position.x/TILE_SIZE);
    int tileY = (int)roundf(position.y/TILE_SIZE);
    return {tileX, tileY};
}

void Board::setTileTaken(std::vector<Shape3D> &obstacles, std::vector<std::vector<Tile>> &tileMap) {

    for (auto &obstacle : obstacles) {

        sf::Vector2f size = {obstacle.getBaseRect().getSize().x/Board::TILE_SIZE, obstacle.getBaseRect().getSize().y/Board::TILE_SIZE};
        sf::Vector2f position = {obstacle.getBaseRect().getPosition().x/Board::TILE_SIZE, obstacle.getBaseRect().getPosition().y/Board::TILE_SIZE};

        for (int x = 0; x < size.x; ++x) {
            for (int y = 0; y < size.y; ++y) {
                tileMap[position.x + x][position.y + y].isTaken = true;
                //std::cout << "Tile " << position.x + x << "," << position.y + y << " set to TAKEN" << std::endl;
            }
        }
    }
}

void Board::setTileFree(std::vector<Shape3D> &obstacles, std::vector<std::vector<Tile>> &tileMap) {

    for (auto &obstacle : obstacles) {

        sf::Vector2f size = {obstacle.getBaseRect().getSize().x/Board::TILE_SIZE, obstacle.getBaseRect().getSize().y/Board::TILE_SIZE};
        sf::Vector2f position = {obstacle.getBaseRect().getPosition().x/Board::TILE_SIZE, obstacle.getBaseRect().getPosition().y/Board::TILE_SIZE};

        for (int x = 0; x < size.x; ++x) {
            for (int y = 0; y < size.y; ++y) {
                tileMap[position.x + x][position.y + y].isTaken = false;
            }
        }
    }
}
