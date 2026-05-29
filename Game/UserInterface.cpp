//
// Created by Kent on 03.11.2017.
//
#include "UserInterface.h"

#include "../Networking/Network.h"
#include "../Support/ActionHandler.h"
#include "Board/Board.h"
#include "Input.h"
#include "Pathfinding/Pathfinding.h"
#include <stdexcept>

userInterface::userInterface(
    sf::RenderWindow& window, Network* network, MatchState* matchState, std::vector<unitBase>* units) {
    this->network    = network;
    this->matchState = matchState;
    view             = window.getView();
    this->units      = units;

    endTurnButton = std::make_shared<Button>();
    endTurnButton->setText("end (P)");
    endTurnButton->setTextColor(sf::Color::White);
    endTurnButton->setButtonColor(sf::Color::Black);
    endTurnButton->setPosition(
        {window.getDefaultView().getSize().x - endTurnButton->getButtonGlobalBounds().size.x * 1.2f,
            (window.getDefaultView().getCenter().y / 2) * 3.715f});

    moveButton = std::make_shared<Button>();
    moveButton->setText("move (M)");
    moveButton->setTextColor(sf::Color::White);
    moveButton->setButtonColor(sf::Color::Black);
    moveButton->setPosition({200, (window.getDefaultView().getCenter().y / 2) * 3.5f});

    shootButton = std::make_shared<Button>();
    shootButton->setText("shoot (K)");
    shootButton->setTextColor(sf::Color::White);
    shootButton->setButtonColor(sf::Color::Black);
    shootButton->setPosition({moveButton->getPosition().x + (moveButton->getButtonGlobalBounds().size.x + 100),
        (window.getDefaultView().getCenter().y / 2) * 3.5f});

    grenadeButton = std::make_shared<Button>();
    grenadeButton->setText("grenade (G)");
    grenadeButton->setTextColor(sf::Color::White);
    grenadeButton->setButtonColor(sf::Color::Black);
    grenadeButton->setPosition({shootButton->getPosition().x + (shootButton->getButtonGlobalBounds().size.x + 150),
        (window.getDefaultView().getCenter().y / 2) * 3.5f});

    highlighted = nothing;
    selected    = nothing;

    healthBar.setFillColor(sf::Color::Green);
    healthBar.setOutlineColor(sf::Color::White);
    healthBar.setOutlineThickness(-1);
    healthBar.setSize(sf::Vector2f(Board::TILE_SIZE, Board::TILE_SIZE / 5));

    if (!font.openFromFile("Data/Fonts/neuropol.ttf")) {
        throw std::runtime_error("Could not load font: Data/Fonts/neuropol.ttf");
    }
    actionPoints.emplace(font);
    actionPoints->setCharacterSize(8);
    actionPoints->setOutlineThickness(1);
    actionPoints->setOutlineColor(sf::Color::White);

    titleText.emplace(font);
    titleText->setCharacterSize(120);
    titleText->setOutlineThickness(1);
    titleText->setPosition({window.getDefaultView().getCenter().x - 250, 25});
    titleText->setStyle(sf::Text::Style::Bold);
}

void userInterface::process(sf::Event event, sf::RenderWindow& window, Board& gameBoard) {
    if (!matchState->turn) {
        return;
    }

    if (event.is<sf::Event::MouseMoved>()) {
        Tile* testTile = input::getMouseOverTile(gameBoard.tileMap, window);

        bool found = false;
        for (int i = 0; i < (int) units->size(); i++) {
            if (units->at(i).shape.getGlobalBounds().findIntersection(testTile->getGlobalBounds())
                && units->at(i).player == matchState->playerNumber && units->at(i).alive) {
                highlithedUnitID = i;
                highlighted      = unit;
                found            = true;
            }
        }

        if (selected == move) {
            resetPath();
            unitBase selectedUnit = units->at(selectedUnitID);
            if (selectedUnit.path.empty()) {
                Tile unitPosition = gameBoard.tileMap.at(selectedUnit.position.x).at(selectedUnit.position.y);

                std::vector<sf::Vector2i> path = std::vector<sf::Vector2i>();
                if (!selectedUnit.isMoving) {
                    path = Pathfinding::getPath(unitPosition, *testTile, selectedUnit.movementMap,
                        selectedUnit.actionPoints, gameBoard.tileMap);
                }
                for (auto tilePos : path) {
                    Tile* tile = &gameBoard.tileMap.at(tilePos.x).at(tilePos.y);
                    tile->setHighlighted();
                    highlightedPath.push_back(tile);
                }
            }
        }

        if (endTurnButton->getButtonGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {
            endTurnButton->setTextColor(sf::Color::Yellow);
            highlighted = end;

        } else if (moveButton->getButtonGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {
            moveButton->setTextColor(sf::Color::Yellow);
            highlighted = move;

        } else if (shootButton->getButtonGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {
            shootButton->setTextColor(sf::Color::Yellow);
            highlighted = shoot;

        } else if (grenadeButton->getButtonGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {
            grenadeButton->setTextColor(sf::Color::Yellow);
            highlighted = grenade;

        } else {
            endTurnButton->setTextColor(sf::Color::White);
            moveButton->setTextColor(selected == move ? sf::Color::Red : sf::Color::White);
            shootButton->setTextColor(selected == shoot ? sf::Color::Red : sf::Color::White);
            grenadeButton->setTextColor(selected == grenade ? sf::Color::Red : sf::Color::White);

            if (!found) {
                highlighted      = nothing;
                highlithedUnitID = -1;
            }
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && selectedUnitID != -1) {
        selected = nothing;
        units->at(selectedUnitID).shape.setOutlineThickness(0);
        selectedUnitID = -1;
    }

    const auto* keyReleased         = event.getIf<sf::Event::KeyReleased>();
    const sf::Keyboard::Key keyCode = keyReleased ? keyReleased->code : sf::Keyboard::Key::Unknown;

    if ((event.is<sf::Event::MouseButtonReleased>() && highlighted != nothing) || (selectedUnitID != -1 && keyReleased)
        || (keyCode == sf::Keyboard::Key::P)) {
        if (highlighted == unit && units->at(highlithedUnitID).player == matchState->playerNumber) {
            if (selectedUnitID != -1) {
                units->at(selectedUnitID).shape.setOutlineThickness(0);
            }
            selectedUnitID = highlithedUnitID;
            units->at(selectedUnitID).shape.setOutlineThickness(2);
            selected = move;
            moveButton->setTextColor(sf::Color::Red);
            endTurnButton->setTextColor(sf::Color::White);
            shootButton->setTextColor(sf::Color::White);
            grenadeButton->setTextColor(sf::Color::White);
            resetPath();
        }

        else if (highlighted == end || keyCode == sf::Keyboard::Key::P) {
            for (auto& unit : *units) {
                unit.turn = false;
            }
            if (selectedUnitID != -1) {
                units->at(selectedUnitID).shape.setOutlineThickness(0);
            }

            selectedUnitID = -1;
            selected       = end;
            resetPath();
            endTurnButton->setTextColor(sf::Color::White);

            {
                std::lock_guard lock(network->clientsMutex);
                if (!network->isHost() || !network->clients.empty()) {
                    matchState->titleString = "Opponent's turn";
                    matchState->titleColor  = sf::Color::Blue;
                }
            }

            if (!network->isHost()) {
                sf::Packet packet;
                packet << FUNCTION_END_OF_TURN << matchState->playerNumber;
                network->sendPacket(packet);
                matchState->turn = false;
            } else {
                actionHandler::nextTurn(matchState->playerNumber, *network, units, *matchState);
            }

        } else if (highlighted == move || keyCode == sf::Keyboard::Key::M) {
            moveButton->setTextColor(sf::Color::Red);
            endTurnButton->setTextColor(sf::Color::White);
            shootButton->setTextColor(sf::Color::White);
            grenadeButton->setTextColor(sf::Color::White);

            selected = move;

        } else if (highlighted == shoot || keyCode == sf::Keyboard::Key::K) {
            moveButton->setTextColor(sf::Color::White);
            endTurnButton->setTextColor(sf::Color::White);
            shootButton->setTextColor(sf::Color::Red);
            grenadeButton->setTextColor(sf::Color::White);

            resetPath();
            selected = shoot;

        } else if (highlighted == grenade || keyCode == sf::Keyboard::Key::G) {
            moveButton->setTextColor(sf::Color::White);
            endTurnButton->setTextColor(sf::Color::White);
            shootButton->setTextColor(sf::Color::White);
            grenadeButton->setTextColor(sf::Color::Red);

            resetPath();
            selected = grenade;
        }
    } else if (event.is<sf::Event::MouseButtonReleased>() && selected != nothing && selectedUnitID != -1) {
        if (units->at(selectedUnitID).turn && units->at(selectedUnitID).alive) {
            switch (selected) {
            case move:
                {
                    Tile* tile    = input::getMouseOverTile(gameBoard.tileMap, window);
                    const float x = tile->getPosition().x / Board::TILE_SIZE;
                    const float y = tile->getPosition().y / Board::TILE_SIZE;
                    if (network->isHost()) {
                        auto packets = actionHandler::Move(units->at(selectedUnitID), x, y, gameBoard.tileMap);
                        for (auto& p : packets) {
                            network->sendPacket(p);
                        }
                    } else {
                        sf::Packet request;
                        auto& u = units->at(selectedUnitID);
                        request << REQUEST_MOVE << u.id << u.player << x << y;
                        network->sendPacket(request);
                    }
                    break;
                }

            case shoot:
                {
                    const sf::Vector2f target = input::getMouse(window);
                    if (network->isHost()) {
                        auto packets = actionHandler::Shoot(units->at(selectedUnitID), target, *units, gameBoard,
                            units->at(selectedUnitID).getWeapon()->type);
                        for (auto& p : packets) {
                            network->sendPacket(p);
                        }
                        actionHandler::victory(*units, network, *matchState);
                    } else {
                        sf::Packet request;
                        auto& u = units->at(selectedUnitID);
                        request << REQUEST_SHOOT << u.id << u.player << target.x << target.y
                                << static_cast<int>(u.getWeapon()->type);
                        network->sendPacket(request);
                    }
                    break;
                }

            case grenade:
                {
                    const sf::Vector2f target = input::getMouse(window);
                    if (network->isHost()) {
                        auto packets = actionHandler::Shoot(
                            units->at(selectedUnitID), target, *units, gameBoard, weaponBase::grenadeType);
                        for (auto& p : packets) {
                            network->sendPacket(p);
                        }
                        actionHandler::victory(*units, network, *matchState);
                    } else {
                        sf::Packet request;
                        auto& u = units->at(selectedUnitID);
                        request << REQUEST_SHOOT << u.id << u.player << target.x << target.y
                                << static_cast<int>(weaponBase::grenadeType);
                        network->sendPacket(request);
                    }
                    break;
                }

            case nothing:
            case end:
            case unit:
                break;
            }
        }
    }
}

void userInterface::draw(sf::RenderWindow& window) {
    view = window.getView();
    window.setView(window.getDefaultView());

    if (matchState->turn) {
        endTurnButton->draw(window);
    }

    if (selectedUnitID != -1 && units->at(selectedUnitID).turn && units->at(selectedUnitID).alive) {
        moveButton->draw(window);
        shootButton->draw(window);
        grenadeButton->draw(window);
    }
    if (titleText) {
        titleText->setString(matchState->titleString);
        titleText->setFillColor(matchState->titleColor);
        window.draw(*titleText);
    }
    window.setView(view);

    for (auto& unit : *units) {
        if (unit.alive && unit.visible) {
            displayStats(&unit, window);
        }
    }
}

void userInterface::displayStats(const unitBase* unit, sf::RenderWindow& window) {
    drawHealthBar(unit, window);
    drawActionPoints(unit, window);
}

void userInterface::drawHealthBar(const unitBase* unit, sf::RenderWindow& window) {
    if (unit->health < unit->maxHealth / 4) {
        healthBar.setFillColor(sf::Color::Red);
    } else if (unit->health < unit->maxHealth / 2) {
        healthBar.setFillColor(sf::Color::Yellow);
    } else {
        healthBar.setFillColor(sf::Color::Green);
    }

    healthBar.setPosition(
        {unit->shape.getPosition().x - Board::TILE_SIZE / 2, unit->shape.getPosition().y - Board::TILE_SIZE / 2 - 6});

    const float healthPercentage = static_cast<float>(unit->health) / unit->maxHealth;
    healthBar.setSize(sf::Vector2f(Board::TILE_SIZE * healthPercentage, Board::TILE_SIZE / 5));

    window.draw(healthBar);
}

void userInterface::drawActionPoints(const unitBase* unit, sf::RenderWindow& window) {
    actionPoints->setFillColor(unit->actionPoints > 0 ? sf::Color::Cyan : sf::Color::Red);
    actionPoints->setPosition({unit->shape.getPosition().x - actionPoints->getLocalBounds().size.x / 2 + 1,
        unit->shape.getPosition().y - Board::TILE_SIZE - 4});

    actionPoints->setString(std::to_string(unit->actionPoints) + " / " + std::to_string(unit->maxAP));

    window.draw(*actionPoints);
}

void userInterface::resetPath() {
    for (const auto tile : highlightedPath) {
        tile->setNotHighlighted();
    }
    highlightedPath.clear();
}
