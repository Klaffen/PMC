#ifndef DAT220_PROJECT_GAME_STATE_SETTINGS_H
#define DAT220_PROJECT_GAME_STATE_SETTINGS_H

#include <iostream>
#include <Support/Button.h>
#include "SFML/Graphics.hpp"
#include "State.h"

class Settings: public State{

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
    sf::Event event{};
    bool settingsInit = false;

    sf::Texture backgroundTexture;
    sf::Sprite background;
    sf::Vector2f backgroundScale;
    float backgroundScaleX;
    float backgroundScaleY;

    sf::Texture keybindTexture;
    sf::Sprite keybind;
    float keybindScaleX;
    float keybindScaleY;
    /**
     * Available resolutions
     */
    std::vector<sf::VideoMode> VMresolutions;

    sf::RectangleShape mousePointer;

    sf::Font font;

    unsigned int textSizeLarge = 56;
    unsigned int textSizeMedium = 44;
    unsigned int textSizeSmall = 32;

    float textSpaceFactor = 1.5f;

    sf::Text fullscreen;
    sf::Text toggleWindowedFullscreen;
    bool isFullscreen;

    sf::Text resolution;
    sf::Text resNumbers;
    bool resolutionChanged;

    sf::Text sound;
    sf::Text toggleSoundOffOn;

    sf::Text keybinds;

    sf::RectangleShape lineKeybind;

    ButtonPtr fullscreenLEFT;
    ButtonPtr fullscreenRIGHT;

    ButtonPtr resolutionLEFT;
    ButtonPtr resolutionRIGHT;

    ButtonPtr soundLEFT;
    ButtonPtr soundRIGHT;

    ButtonPtr back;

    /**
     * Setup for text
     * @param window Window to position text
     */
    void textLablesInit(sf::RenderWindow &window);
    /**
     * Scale text based on resolution
     * @param window Window to check size
     */
    void resolutionTextSize(sf::RenderWindow &window);
    /**
     * Updates the text positions
     * @param window
     */
    void updateTextPosition(sf::RenderWindow &window);
    /**
     * Setup for buttons
     * @param window Window to position buttons
     */
    void buttonInit(sf::RenderWindow &window);
    /**
     * Handles mouse input in the window
     * @param mousePointer The mouse pointer
     * @param window Window to check mouse pointer interactions with
     * @return True when changing state
     */
    bool mouseInput(sf::RectangleShape &mousePointer, sf::RenderWindow &window);
};

typedef std::shared_ptr<Settings> SettingsPtr;

#endif //DAT220_PROJECT_GAME_STATE_SETTINGS_H
