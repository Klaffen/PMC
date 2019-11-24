#ifndef DAT220_PROJECT_GAME_STATE_MENU_H
#define DAT220_PROJECT_GAME_STATE_MENU_H

#include <iostream>
#include <Support/Button.h>
#include "SFML/Graphics.hpp"
#include "State.h"

//TODO: Make a general design for menu
class Menu: public State{

public:
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

private:
    sf::View menuView;
    sf::Event event{};

    bool menuInit = false;

    sf::RectangleShape mousePointer;

    sf::Texture backgroundTexture;
    sf::Sprite background;
    float backgroundScaleX;
    float backgroundScaleY;

    ButtonPtr playButton;
    ButtonPtr squadButton;
    ButtonPtr settingsButton;
    ButtonPtr exitButton;

    /**
     * Takes care of button initialization
     * @param window Window to set button positions
     */
    void buttonInit(const sf::RenderWindow &window);
    /**
     * Handles mouse input in the window
     * @param mousePointer The mouse pointer
     * @param window Window to check mouse pointer interactions with
     * @return True when changing state
     */
    bool mouseInput(sf::RectangleShape &mousePointer, const sf::RenderWindow &window);
};

typedef std::shared_ptr<Menu> MenuPtr;

#endif //DAT220_PROJECT_GAME_STATE_MENU_H
