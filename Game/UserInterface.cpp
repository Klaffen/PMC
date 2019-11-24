//
// Created by Kent on 03.11.2017.
//
#include "UserInterface.h"

userInterface::userInterface(sf::RenderWindow &window, Network *network, std::vector<unitBase> *units) {
    this->network = network;
    view = window.getView();
    this->units = units;

    endTurnButton = std::make_shared<Button>();
    endTurnButton->setText("end (P)");
    endTurnButton->setTextColor(sf::Color::White);
    endTurnButton->setButtonColor(sf::Color::Black);
    endTurnButton->setPosition({window.getDefaultView().getSize().x - endTurnButton->getButtonGlobalBounds().width * 1.2f, (window.getDefaultView().getCenter().y / 2) * 3.715f});

    moveButton = std::make_shared<Button>();
    moveButton->setText("move (M)");
    moveButton->setTextColor(sf::Color::White);
    moveButton->setButtonColor(sf::Color::Black);
    moveButton->setPosition({200, (window.getDefaultView().getCenter().y / 2) * 3.5f});

    shootButton = std::make_shared<Button>();
    shootButton->setText("shoot (K)");
    shootButton->setTextColor(sf::Color::White);
    shootButton->setButtonColor(sf::Color::Black);
    shootButton->setPosition({moveButton->getPosition().x + (moveButton->getButtonGlobalBounds().width + 100), (window.getDefaultView().getCenter().y / 2) * 3.5f});

    grenadeButton = std::make_shared<Button>();
    grenadeButton->setText("grenade (G)");
    grenadeButton->setTextColor(sf::Color::White);
    grenadeButton->setButtonColor(sf::Color::Black);
    grenadeButton->setPosition({shootButton->getPosition().x + (shootButton->getButtonGlobalBounds().width + 150), (window.getDefaultView().getCenter().y / 2) * 3.5f});

    highlighted = nothing;
    selected = nothing;

    healthBar.setFillColor(sf::Color::Green);
    healthBar.setOutlineColor(sf::Color::White);
    healthBar.setOutlineThickness(-1);
    healthBar.setSize(sf::Vector2f(6.4,6.4));

    font.loadFromFile("Data/Fonts/neuropol.ttf");
    actionPoints.setFont(font);
    actionPoints.setCharacterSize(16);
    actionPoints.setOutlineThickness(1);
    actionPoints.setOutlineColor(sf::Color::White);

    network->title.setFont(font);
    network->title.setCharacterSize(120);
    network->title.setOutlineThickness(1);
    network->title.setPosition(window.getDefaultView().getCenter().x - 250, 25);
    network->title.setStyle(sf::Text::Style::Bold);

    equipedWeapon.setFont(font);
    equipedWeapon.setCharacterSize(16);
    equipedWeapon.setOutlineColor(sf::Color::White);
}

void userInterface::proccess(const sf::Event &event, sf::RenderWindow &window, Board &gameBoard) {
    if (!network->turn)
        return;

    if (event.type == sf::Event::EventType::MouseMoved) {
        Tile *testTile = input::getMouseOverTile(gameBoard.tileMap, window);

        bool found = false;
        for (int i = 0; i < (int) units->size(); i++) {
            if (units->at(i).shape.getGlobalBounds().intersects(testTile->getGlobalBounds()) && units->at(i).player == network->playerNumber && units->at(i).alive){
                highlithedUnitID = i;
                highlighted = unit;
                found = true;
            }
        }
        if (selected == move){
            resetPath();
            unitBase selectedUnit = units->at(selectedUnitID);
            if (selectedUnit.path.empty()) {
                Tile unitPosition = gameBoard.tileMap.at(selectedUnit.position.x).at(selectedUnit.position.y);

                std::vector<sf::Vector2i> path = std::vector<sf::Vector2i>();
                if (!selectedUnit.isMoving) {
                    path = Pathfinding::getPath(unitPosition,
                                                *testTile,
                                                selectedUnit.movementMap,
                                                selectedUnit.actionPoints,
                                                gameBoard.tileMap);
                }
                for (auto tilePos : path) {
                    Tile *tile = &gameBoard.tileMap.at(tilePos.x).at(tilePos.y);
                    tile->setHighlighted();
                    highlightedPath.push_back(tile);
                }
            }
        }

        if (endTurnButton->getButtonGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) && selected != end) {
            endTurnButton->setTextColor(sf::Color::Yellow);
            highlighted  = end;
        }
        else if (moveButton->getButtonGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) && selected != move) {
            moveButton->setTextColor(sf::Color::Yellow);
            highlighted  = move;
        }
        else if (shootButton->getButtonGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) && selected != shoot) {
            shootButton->setTextColor(sf::Color::Yellow);
            highlighted  = shoot;
        }
        else if (grenadeButton->getButtonGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) && selected != grenade) {
            grenadeButton->setTextColor(sf::Color::Yellow);
            highlighted  = grenade;
        }
        else {
            if (selected != end)
                endTurnButton->setTextColor(sf::Color::White);
            if (selected != move)
                moveButton->setTextColor(sf::Color::White);
            if (selected != shoot)
                shootButton->setTextColor(sf::Color::White);
            if (selected != grenade)
                grenadeButton->setTextColor(sf::Color::White);

            if (!found) {
                highlighted = nothing;
                highlithedUnitID = -1;
            }
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && selectedUnitID != -1) {
        selected = nothing;
        units->at(selectedUnitID).shape.setOutlineThickness(0);
        selectedUnitID = -1;
    }

    if ((event.type == sf::Event::MouseButtonReleased && highlighted != nothing) || (selectedUnitID != -1 && event.type == sf::Event::KeyReleased) || (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::P)) {
        if (highlighted == unit && units->at(highlithedUnitID).player == network->playerNumber) {
            if (selectedUnitID != -1)
                units->at(selectedUnitID).shape.setOutlineThickness(0);
            selectedUnitID = highlithedUnitID;
            units->at(selectedUnitID).shape.setOutlineThickness(2);
            selected = move;
            moveButton->setTextColor(sf::Color::Red);
            endTurnButton->setTextColor(sf::Color::White);
            shootButton->setTextColor(sf::Color::White);
            grenadeButton->setTextColor(sf::Color::White);
            resetPath();
        }

        else if (highlighted == end || (event.key.code == sf::Keyboard::P)) {
            for (auto &unit : *units) {
                unit.restoreAP();
                unit.turn = false;
            }
            if (selectedUnitID != -1)
                units->at(selectedUnitID).shape.setOutlineThickness(0);

            selectedUnitID = -1;
            selected = end;
            resetPath();
            endTurnButton->setTextColor(sf::Color::White);

            if (!network->HOST || !network->clients.empty()) {
                network->title.setString("Opponent's turn");
                network->title.setFillColor(sf::Color::Blue);
            }

            if (!network->HOST) {
                sf::Packet packet;
                packet << 1010 << network->playerNumber;
                network->sendPacket(packet, false);
                network->turn = false;
            }
            else
                actionHandler::nextTurn(network->playerNumber, *network, units);
        }
        else if (highlighted == move || event.key.code == sf::Keyboard::M) {
            moveButton->setTextColor(sf::Color::Red);
            endTurnButton->setTextColor(sf::Color::White);
            shootButton->setTextColor(sf::Color::White);
            grenadeButton->setTextColor(sf::Color::White);

            selected = move;
        }
        else if ( (highlighted == shoot || event.key.code == sf::Keyboard::K)) {
            moveButton->setTextColor(sf::Color::White);
            endTurnButton->setTextColor(sf::Color::White);
            shootButton->setTextColor(sf::Color::Red);
            grenadeButton->setTextColor(sf::Color::White);

            resetPath();
            selected = shoot;
        }
        else if (highlighted == grenade || event.key.code == sf::Keyboard::G) {
            moveButton->setTextColor(sf::Color::White);
            endTurnButton->setTextColor(sf::Color::White);
            shootButton->setTextColor(sf::Color::White);
            grenadeButton->setTextColor(sf::Color::Red);

            resetPath();
            selected = grenade;
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased && selected != nothing && selectedUnitID != -1) {
        if (units->at(selectedUnitID).turn && units->at(selectedUnitID).alive)
        switch (selected) {
            case move: {
                Tile *tile = input::getMouseOverTile(gameBoard.tileMap, window);
                std::vector<sf::Packet> packets = actionHandler::Move(units->at(selectedUnitID), tile->getPosition().x / 32,
                                                                      tile->getPosition().y / 32, gameBoard.tileMap);
                for (auto packet : packets)
                    network->sendPacket(packet, false);
                break;
            }
            case shoot: {
                sf::Vector2f mousePointer = input::getMouse(window);
                sf::Vector2f mousePosition = sf::Vector2f(mousePointer.x, mousePointer.y);

                std::vector<sf::Packet> packets = actionHandler::Shoot(units->at(selectedUnitID), mousePosition, *units, gameBoard);
                    for (auto packet : packets) {
                        network->sendPacket(packet, false);
                }
                if (network->HOST)
                    actionHandler::victory(*units, network);
                break;
            }
            case grenade: {
                sf::Vector2f mousePointer = input::getMouse(window);
                sf::Vector2f mousePosition = sf::Vector2f(mousePointer.x, mousePointer.y);

                units->at(selectedUnitID).weaponSwap(weaponBase::grenadeType);
                std::vector<sf::Packet> packets = actionHandler::Shoot(units->at(selectedUnitID), mousePosition, *units, gameBoard);
                for (auto packet : packets) {
                    network->sendPacket(packet, false);
                }
                if (network->HOST)
                    actionHandler::victory(*units, network);
                break;
            }
            case nothing:
                break;
            case end:
                break;
            case unit:
                break;
        }
    }
}

void userInterface::draw(sf::RenderWindow &window) {
    view = window.getView();
    window.setView(window.getDefaultView());

    if (network->turn)
        endTurnButton->draw(window);

    if (selectedUnitID != -1 && units->at(selectedUnitID).turn && units->at(selectedUnitID).alive) {
        moveButton->draw(window);
        shootButton->draw(window);
        grenadeButton->draw(window);
    }
    window.draw(network->title);
    window.setView(view);

    for (auto &unit : *units) {
        if (unit.alive && unit.visible)
            displayStats(&unit, window);
    }

}

void userInterface::displayStats(unitBase *unit, sf::RenderWindow &window) {
    if (unit->health < 50)
        healthBar.setFillColor(sf::Color::Red);
    else
        healthBar.setFillColor(sf::Color::Green);

    for (int x = 0; x < unit->health; x+= 15){
        healthBar.setPosition(unit->shape.getPosition().x - Board::TILE_SIZE/2 + (float) (6.4 * x / 15), unit->shape.getPosition().y - Board::TILE_SIZE/2 - 6);
        window.draw(healthBar);
    }

    if (unit->actionPoints > 0)
        actionPoints.setFillColor(sf::Color::Cyan);
    else
        actionPoints.setFillColor(sf::Color::Red);

    actionPoints.setPosition(unit->shape.getPosition().x - actionPoints.getLocalBounds().width/2 + 1, (unit->shape.getPosition().y- (Board::TILE_SIZE)- 10));
    actionPoints.setString(std::to_string(unit->actionPoints));
    window.draw(actionPoints);

/*    if (unit->getWeapon() == 0)
        equipedWeapon.setString("Nothing");
    else if (unit->currentWeapon == 1)
        equipedWeapon.setString("Rifle");
    else if (unit->currentWeapon == 2)
        equipedWeapon.setString("Shotgun");
    else if (unit->currentWeapon == 3)
        equipedWeapon.setString("Grenade");
    equipedWeapon.setPosition(unit->shape.getPosition().x, unit->shape.getPosition().y + unit->shape.getGlobalBounds().height);
    window.draw(equipedWeapon);*/
}

void userInterface::resetPath() {
    for (auto tile : highlightedPath)
        tile->setNotHighlighted();
    highlightedPath.clear();
}
