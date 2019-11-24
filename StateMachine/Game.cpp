#include <Support/ActionHandler.h>
#include <Game/Input.h>
#include <Game/Pathfinding/Pathfinding.h>
#include <SquadBuilder/SquadIO.h>
#include "Game.h"

Game::Game(Network *network) {
    this->network = network;
}

const int Game::viewMoveStep = 6;

//TODO: Make dynamic based on resolution
sf::Vector2f Game::viewStartingSize = {960, 540};
float Game::maxZoom = 1.6;
float Game::minZoom = 0.7;


int Game::enter(sf::RenderWindow &window) {

    //TODO: Handle case when you already have been here before
    view.setSize(960, 540); // 1920 / 2, 1080 / 2
    view.setCenter(0 + view.getSize().x/2, 0 + view.getSize().y/2);
    window.setView(view);
    gameBoard->init(window);

    units = makeTemporaryUnitList();

    network->turn = network->HOST;

    process(window);

    return currentScreenState;
}

void Game::process(sf::RenderWindow &window) {
    userInterface interface(window, network, &units);
    while (window.isOpen()) {
    //Game loop start

        while (window.pollEvent(event)) {
            //Event loop start (Handling inputs)

            if (event.type == sf::Event::Closed) {
                window.close();
                if (!network->HOST)
                    network->clientSocket.disconnect();
                else
                    for (auto &client : network->clients)
                        client->socket.disconnect();
                currentScreenState = screenState::TERMINATE;
                return;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                if (!network->HOST)
                    network->clientSocket.disconnect();
                else
                    for (auto &client : network->clients)
                        client->socket.disconnect();
                currentScreenState = screenState::MENU;
                return;
            }
            interface.proccess(event, window, *gameBoard);
        }
        //Event loop end


        sf::Packet packet = network->receivePacket(false);
        actionHandler::GetRemoteAction(packet, &units, network, *gameBoard);

        for (auto &unit : units)
            unit.process(*gameBoard, window, units);

        gameBoard->Process(window, view);

        drawFrame(window, interface);

    //Game loop end
    }
}

void Game::drawFrame(sf::RenderWindow &window, userInterface &interface) {
    window.clear(sf::Color(12, 0, 42));

    gameBoard->drawTiles(window); //Draw tiles first

    for (auto &unit : units) {
        unit.Draw(window);
    }

    gameBoard->drawObstacles(window); //Draw obstacles last (Unless there is specific reason for not doing it)

    interface.draw(window);
    window.display();
}

std::vector<unitBase> Game::makeTemporaryUnitList() {
    std::vector<unitBase> list;
    list = squadIO::makeUnits("SquadBuilder/squad.txt", network, *gameBoard);
    return list;
}