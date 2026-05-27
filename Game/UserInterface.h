//
// Created by Kent on 03.11.2017.
//

#ifndef DAT220_PROJECT_USERINTERFACE_H
#define DAT220_PROJECT_USERINTERFACE_H

#include "../Support/Button.h"
#include "MatchState.h"
#include "Objects/Units/UnitBase.h"
#include <optional>

#include "SFML/Graphics.hpp"

class Network;
class Board;


class userInterface {
public:
    /**
     * Constructor. Sets up text and buttons
     * @param window Reference to the game window
     * @param network Pointer to the network object
     * @param units Pointer to the unit list
     * @return Returns nothing
     */
    userInterface(sf::RenderWindow& window, Network* network, MatchState* matchState, std::vector<unitBase>* units);

    /**
     * Process user input
     * @param event Reference to event
     * @param window Reference to window
     * @param window Reference to the game board
     * @return Returns nothing
     */
    void proccess(sf::Event event, sf::RenderWindow& window, Board& gameBoard);

    /**
     * Draws the buttons
     * @param window Reference to the game window
     * @return Returns nothing
     */
    void draw(sf::RenderWindow& window);

    Network* network;
    MatchState* matchState;

    ButtonPtr endTurnButton, moveButton, shootButton, grenadeButton;
    enum buttonEnum { nothing, end, move, shoot, grenade, unit };
    buttonEnum highlighted;
    buttonEnum selected;

    std::vector<unitBase>* units;
    int highlithedUnitID = -1;
    int selectedUnitID   = -1;

    sf::RectangleShape healthBar;
    std::optional<sf::Text> actionPoints;
    std::optional<sf::Text> titleText;
    sf::Font font;

    sf::View view;

private:
    std::vector<Tile*> highlightedPath{};

    /**
     * Reset path highlighting
     * @return Returns nothing
     */
    void resetPath();

    /**
     * Draws the stats of all visible units
     * @param unit Pointer to the unit
     * @param window Reference to the game window
     * @return Returns nothing
     */
    void displayStats(unitBase* unit, sf::RenderWindow& window);
};


#endif // DAT220_PROJECT_USERINTERFACE_H
