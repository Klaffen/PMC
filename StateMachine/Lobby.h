#ifndef DAT220_PROJECT_HOST_H
#define DAT220_PROJECT_HOST_H

#include <iostream>
#include <vector>

#include "SFML/Graphics.hpp"

#include "State.h"
#include "../Networking/Network.h"
#include "Support/Button.h"

class Lobby: public State {
public:
    /**
    * Constructor for the lobby state
    * @param network the network object
    * @return Returns nothing
    */
    Lobby(Network *network);
    Network *network;

    /**
     * Loads and sets up the text and buttons for the state
     * @param window Reference to the game window
     * @return Returns the id of the next state
     */
    int enter(sf::RenderWindow &window);

    /**
     * Process all the user input and networking
     * @param window Reference to the game window
     * @return returns nothing
     */
    void process(sf::RenderWindow &window);

    /**
     * Setup for joining or hosting a game
     * @param window Reference to the game window
     * @return Returns nothing
     */
    void lobbySetup(sf::RenderWindow &window);

    sf::Texture backgroundTexture;
    sf::Sprite background;
    sf::Vector2f backgroundScale;
    float backgroundScaleX;
    float backgroundScaleY;

    sf::Font font;

    ButtonPtr hostButton;
    ButtonPtr joinButton;
    ButtonPtr joinManuallyButton;
    ButtonPtr startGameButton;
    sf::Text inputTag, titlebar, gameNameText, userInput, textElement;

    sf::RectangleShape chatBox;

    int startIntervall, endIntervall, position;
    bool manualScroll;
    std::vector<std::string> messageVector;

    int playerCount;

    std::string hostGlobalIP = "";
    std::string hostLocalIP = "";
private:
    sf::Event event{};
};

typedef std::shared_ptr<Lobby> LobbyPtr;
#endif
