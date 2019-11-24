#ifndef DAT220_PROJECT_GAME_STATE_GAME_H
#define DAT220_PROJECT_GAME_STATE_GAME_H

#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <Game/Board/Board.h>
#include <Networking/Network.h>
#include <Game/Vision/Vision.h>
#include "State.h"
#include "Game/Objects/Units/UnitBase.h"
#include "Game/UserInterface.h"

class Game : public State {

public:
    /**
     * Constructor for game state
     * @param network To setup network connection
     */
    Game(Network *network);
    /**
     * Initialization for the state
     * @param window window to manipulate in the state
     * @return retuns what state to change to
     */
    int enter(sf::RenderWindow &window);
    /**
     * Processes user input, makes updates and draw frames
     * @param window window to manipulate in the state
     */
    void process(sf::RenderWindow &window);

    std::vector<unitBase> units;

    Network *network;

    static const int viewMoveStep;

    static sf::Vector2f viewStartingSize;

    static float maxZoom;
    static float minZoom;

private:
    sf::Event event{};
    sf::View view;
    BoardPtr gameBoard = std::make_shared<Board>();
    std::vector<unitBase> makeTemporaryUnitList();
    /**
     * Draws the frame
     * @param window Window to draw to
     * @param interface The UI to draw
     */
    void drawFrame(sf::RenderWindow &window, userInterface &interface);
};

typedef std::shared_ptr<Game> GamePtr;
#endif //DAT220_PROJECT_GAME_STATE_GAME_H
