#ifndef DAT220_PROJECT_SQUAD_H
#define DAT220_PROJECT_SQUAD_H

#include "SFML/Graphics.hpp"

#include "State.h"
#include "SquadBuilder/SquadIO.h"
#include "Support/Button.h"

#include <iostream>
#include <vector>
#include <string>

#define PRICE_VISION 10
#define PRICE_HEALTH 10
#define PRICE_ACTIONPOINT 10
#define PRICE_WEAPON 20
#define PRICE_UNIT 50

struct baseUnit {
    int vision = 1;
    int health = 15;
    int actionPoints = 1;

    int weaponId = 0;

    int unitCost = 0;
};

class Squad: public State {
public:
    int enter(sf::RenderWindow &window);

private:
    sf::Event event{};

    bool squadInit = false;

    /**
     * Process the user input
     * @param window Reference to the game window
     * @return Returns nothing
     */
    void process(sf::RenderWindow &window);

    /**
     * Draw all the buttons
     * @param window Reference to the game window
     * @return Returns nothing
     */
    void draw(sf::RenderWindow &window);

    /**
     * Update the text with new values
     * @return Returns nothing
     */
    void updateValues();
    void textInit(sf::RenderWindow &window);
    void buttonInit(sf::RenderWindow &window);
    void updatePositions(sf::RenderWindow &window);

    sf::Texture backgroundTexture;
    sf::Sprite background;
    sf::Vector2f backgroundScale;
    float backgroundScaleX;
    float backgroundScaleY;

    ButtonPtr backButton;
    ButtonPtr saveButton;
    sf::Text saved;
    int saveDrawTime;

    /**
     * Process the users mouse click
     * @return Returns nothing
     */
    void mouseClick();

    /**
     * Check if the mouse intersected any buttons on click
     * @param window Reference to mouse coordinates
     * @return Returns the type of the button pressed
     */
    int buttonCollission(sf::Vector2i &mouse);

    sf::Vector2i mousePosition;
    sf::RectangleShape hitbox;
    int collision;

    sf::Font font;
    unsigned int textSize;

    /**
     * Add a new unit to the unit list
     * @param window Reference to a new unit
     * @return Returns nothing
     */
    void addUnit(baseUnit unit = baseUnit());

    /**
     * Remove unit from list
     * @return Returns nothing
     */
    void removeUnit();

    /**
     * Select unit from unit list
     * @param window Reference to mouse coordinates
     * @return Returns nothing
     */
    void selectUnit(sf::Vector2i &mouse);
    std::vector<baseUnit> unitList;
    int selectedUnitId;
    sf::Text unitListTitle;
    std::vector<sf::Text> unitTexts;
    ButtonPtr addUnitButton;
    ButtonPtr removeUnitButton;

    sf::Text vision, health, actionPoints, weapon, selectedUnit;
    std::vector<sf::Text> plussMinus;

    sf::Color highlighColor;
    sf::Color selectedColor;
    sf::Color textColor;

    int buildPoints;
    sf::Text buildPointsText;
    /**
     * Convert units read from file to the format used to display units
     * @param window Reference to units read from file
     * @return Returns a converted unit list
     */
    std::vector<unitBase::unitClass> unitConversion(std::vector<baseUnit>);

    /**
     * Add units to visible unit list
     * @param window Reference to unit list
     * @return Returns nothing
     */
    void getSquad(std::vector<unitBase::unitClass>);
};

typedef std::shared_ptr<Squad> SquadPtr;

#endif