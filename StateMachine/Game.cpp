#include "Game.h"

#include "../Game/Input.h"
#include "../Game/Pathfinding/Pathfinding.h"
#include "../SquadBuilder/SquadIO.h"
#include "../Support/ActionHandler.h"

Game::Game(Network* network) {
    this->network = network;
}

const int Game::viewMoveStep = 6;

// TODO: Make dynamic based on resolution
sf::Vector2f Game::viewStartingSize = {960, 540};
float Game::maxZoom                 = 1.6;
float Game::minZoom                 = 0.7;


int Game::enter(sf::RenderWindow& window) {

    // TODO: Handle case when you already have been here before
    view.setSize({960.f, 540.f}); // 1920 / 2, 1080 / 2
    view.setCenter({view.getSize().x / 2, view.getSize().y / 2});
    window.setView(view);
    gameBoard->init(window);

    units = makeTemporaryUnitList();

    for (const auto& unit : units) {
        if (unit.player == matchState.playerNumber) {
            view.setCenter(unit.shape.getPosition());
            window.setView(view);
            break;
        }
    }

    matchState.turn = network->isHost();

    process(window);

    return currentScreenState;
}

void Game::process(sf::RenderWindow& window) {
    userInterface interface(window, network, &matchState, &units);
    while (window.isOpen()) {
        // Game loop start

        while (const auto event = window.pollEvent()) {
            // Event loop start (Handling inputs)

            if (event->is<sf::Event::Closed>()) {
                window.close();
                if (!network->isHost()) {
                    network->clientSocket.disconnect();
                } else {
                    std::lock_guard lock(network->clientsMutex);
                    for (auto& client : network->clients) {
                        client->socket.disconnect();
                    }
                }
                currentScreenState = TERMINATE;
                return;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                if (!network->isHost()) {
                    network->clientSocket.disconnect();
                } else {
                    std::lock_guard lock(network->clientsMutex);
                    for (auto& client : network->clients) {
                        client->socket.disconnect();
                    }
                }
                currentScreenState = MENU;
                return;
            }
            interface.process(*event, window, *gameBoard);
        }
        // Event loop end


        sf::Packet packet = network->receivePacket();
        actionHandler::GetRemoteAction(packet, &units, network, *gameBoard, matchState);

        for (auto& unit : units) {
            unit.process(*gameBoard, window, units);
        }

        gameBoard->Process(window, view);

        drawFrame(window, interface);
        // Game loop end
    }
}

void Game::drawFrame(sf::RenderWindow& window, userInterface& interface) {
    window.clear(sf::Color(12, 0, 42));

    gameBoard->drawTiles(window); // Draw tiles first

    for (auto& unit : units) {
        unit.Draw(window);
    }

    gameBoard->drawObstacles(window); // Draw obstacles last (Unless there is specific reason for not doing it)

    interface.draw(window);
    window.display();
}

std::vector<unitBase> Game::makeTemporaryUnitList() {
    return squadIO::makeUnits("SquadBuilder/squad.txt", matchState.playerNumber, network, *gameBoard);
}
