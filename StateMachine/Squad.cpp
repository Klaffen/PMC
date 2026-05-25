#include "Squad.h"

#include "../SquadBuilder/SquadIO.h"

int Squad::enter(sf::RenderWindow &window) {
    if (!squadInit) {
        if (const bool loadedBGTexture = backgroundTexture.loadFromFile("Data/Images/background_blurred.png"); !
            loadedBGTexture) {
            throw std::runtime_error("Could not load background texture!");
        }

        backgroundScaleX = (float) (window.getSize().x) / (float) (backgroundTexture.getSize().x);
        backgroundScaleY = (float) (window.getSize().y) / (float) (backgroundTexture.getSize().y);
        background.emplace(backgroundTexture);
        background->setScale({backgroundScaleX, backgroundScaleY});

        if (const bool loadedFont = font.openFromFile("Data/Fonts/Inconsolata-Regular.ttf"); !loadedFont) {
            throw std::runtime_error("Could not load font!");
        }

        textSize = 68;

        highlighColor = sf::Color::Cyan;
        selectedColor = sf::Color::White;
        textColor = sf::Color::White;

        buildPoints = MAX_BUILD_POINTS;
        getSquad(squadIO::readFile("SquadBuilder/squad.txt"));

        selectedUnitId = 0;
        if (unitList.empty())
            addUnit();

        unitTexts.at(0).setFillColor(sf::Color::Red);

        textInit(window);
        buttonInit(window);

        updateValues();

        for (int i = 0; i < 4; i++) {
            sf::Text minus(font);
            minus.setCharacterSize(textSize + 32);
            minus.setFillColor(textColor);
            minus.setPosition({
                health->getPosition().x + health->getGlobalBounds().size.x + 50,
                vision->getPosition().y + (100 * float(i)) - 24
            });
            minus.setString("-");

            sf::Text pluss(font);
            pluss.setCharacterSize(textSize + 32);
            pluss.setFillColor(textColor);
            pluss.setPosition({
                health->getPosition().x + health->getGlobalBounds().size.x + 110,
                vision->getPosition().y + (100 * float(i)) - 24
            });
            pluss.setString("+");

            plussMinus.push_back(pluss);
            plussMinus.push_back(minus);
        }

        squadInit = true;
    }

    updatePositions(window);
    collision = 0;
    process(window);
    return currentScreenState;
}

void Squad::process(sf::RenderWindow &window) {
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                currentScreenState = TERMINATE;
                return;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                currentScreenState = MENU;
                return;
            }

            if (event->is<sf::Event::MouseMoved>()) {
                mousePosition = sf::Mouse::getPosition(window);
                collision = buttonCollission(mousePosition);
            }
            if (collision != 0 && event->is<sf::Event::MouseButtonReleased>()) {
                if (collision == 5)
                    selectUnit(mousePosition);

                else if (collision == 7) {
                    currentScreenState = MENU;
                    return;
                } else
                    mouseClick();

                updateValues();
            }
        }

        draw(window);
    }
}

void Squad::draw(sf::RenderWindow &window) {
    window.clear(sf::Color(12, 0, 42));
    window.draw(*background);

    window.draw(*selectedUnit);
    window.draw(*vision);
    window.draw(*health);
    window.draw(*actionPoints);
    window.draw(*weapon);
    window.draw(*buildPointsText);

    window.draw(*unitListTitle);

    addUnitButton->draw(window);
    removeUnitButton->draw(window);

    backButton->draw(window);
    saveButton->draw(window);

    for (const auto &unitText: unitTexts) {
        window.draw(unitText);
    }

    for (const auto &plussMinu: plussMinus)
        window.draw(plussMinu);

    if (saveDrawTime > 0) {
        window.draw(*saved);
        saveDrawTime--;
    }

    window.display();
}

void Squad::textInit(sf::RenderWindow &window) {
    selectedUnit.emplace(font);
    selectedUnit->setCharacterSize(textSize + 20);
    selectedUnit->setFillColor(selectedColor);
    selectedUnit->setPosition({window.getView().getCenter().x - 150, 80});
    selectedUnit->setString("Unit: 1");

    unitListTitle.emplace(font);
    unitListTitle->setCharacterSize(textSize);
    unitListTitle->setStyle(sf::Text::Style::Underlined);
    unitListTitle->setFillColor(textColor);
    unitListTitle->setPosition({50, 50});
    unitListTitle->setString("Unit List");

    vision.emplace(font);
    vision->setCharacterSize(textSize);
    vision->setFillColor(textColor);
    vision->setPosition({window.getView().getCenter().x - 350, window.getView().getCenter().y - 200});

    health.emplace(font);
    health->setCharacterSize(textSize);
    health->setFillColor(textColor);
    health->setPosition({window.getView().getCenter().x - 350, window.getView().getCenter().y - 100});

    actionPoints.emplace(font);
    actionPoints->setCharacterSize(textSize);
    actionPoints->setFillColor(textColor);
    actionPoints->setPosition({window.getView().getCenter().x - 350, window.getView().getCenter().y});

    weapon.emplace(font);
    weapon->setCharacterSize(textSize);
    weapon->setFillColor(textColor);
    weapon->setPosition({window.getView().getCenter().x - 350, window.getView().getCenter().y + 100});

    buildPointsText.emplace(font);
    buildPointsText->setCharacterSize(textSize);
    buildPointsText->setFillColor(textColor);
    buildPointsText->setPosition({window.getView().getCenter().x - 350, window.getView().getCenter().y + 300});
}

void Squad::buttonInit(sf::RenderWindow &window) {
    addUnitButton = std::make_shared<Button>();
    addUnitButton->setTextColor(sf::Color::White);
    addUnitButton->setButtonColor(sf::Color::Black);
    addUnitButton->setText("New Unit");
    addUnitButton->setPosition(sf::Vector2f(400, 120));

    removeUnitButton = std::make_shared<Button>();
    removeUnitButton->setTextColor(sf::Color::White);
    removeUnitButton->setButtonColor(sf::Color(202, 46, 85));
    removeUnitButton->setButtonOutlineColor(sf::Color::White);
    removeUnitButton->setText("Delete Unit");
    removeUnitButton->setPosition(
        sf::Vector2f(selectedUnit->getPosition().x + selectedUnit->getGlobalBounds().size.x * 2, 120));

    backButton = std::make_shared<Button>();
    backButton->setTextColor(sf::Color::White);
    backButton->setButtonColor(sf::Color::Black);
    backButton->setText("Back to menu");
    backButton->setPosition(sf::Vector2f(50, window.getView().getSize().y - 100));

    saveButton = std::make_shared<Button>();
    saveButton->setTextColor(sf::Color::White);
    saveButton->setButtonColor(sf::Color(112, 193, 179));
    saveButton->setButtonOutlineColor(sf::Color::White);
    saveButton->setText("Save");
    saveButton->setPosition(sf::Vector2f(window.getView().getSize().x - 300, window.getView().getSize().y - 100));

    saved.emplace(font);
    saved->setCharacterSize(textSize / 2);
    saved->setStyle(sf::Text::Style::Bold);
    saved->setFillColor(sf::Color::Green);
    saved->setPosition({
        saveButton->getPosition().x + saveButton->getButtonGlobalBounds().size.x / 3, saveButton->getPosition().y - 50
    });
    saved->setString("Saved!");
    saveDrawTime = 0;
}

void Squad::updatePositions(sf::RenderWindow &window) {
    selectedUnit->setPosition({window.getView().getCenter().x - 150, 80});
    unitListTitle->setPosition({50, 50});

    vision->setPosition({window.getView().getCenter().x - 350, window.getView().getCenter().y - 200});
    health->setPosition({window.getView().getCenter().x - 350, window.getView().getCenter().y - 100});
    actionPoints->setPosition({window.getView().getCenter().x - 350, window.getView().getCenter().y});
    weapon->setPosition({window.getView().getCenter().x - 350, window.getView().getCenter().y + 100});
    buildPointsText->setPosition({window.getView().getCenter().x - 350, window.getView().getCenter().y + 300});
}

int Squad::buttonCollission(sf::Vector2i &mouse) {
    hitbox.setSize(sf::Vector2f(55, 55));
    hitbox.setPosition({float(mouse.x) - 28, float(mouse.y - 28)});
    for (auto &plussMinu: plussMinus) {
        if (plussMinu.getGlobalBounds().findIntersection(hitbox.getGlobalBounds())) {
            plussMinu.setFillColor(highlighColor);
            if (plussMinu.getString() == "+")
                return 1;

            return 2;
        }
        plussMinu.setFillColor(textColor);
    }

    if (addUnitButton->getButtonGlobalBounds().findIntersection(hitbox.getGlobalBounds())) {
        addUnitButton->setTextColor(sf::Color::Yellow);
        return 3;
    }

    if (removeUnitButton->getButtonGlobalBounds().findIntersection(hitbox.getGlobalBounds())) {
        removeUnitButton->setTextColor(sf::Color::Yellow);
        return 4;
    }

    addUnitButton->setTextColor(sf::Color::White);
    removeUnitButton->setTextColor(sf::Color::White);

    for (unsigned long unit = 0; unit < unitList.size(); unit++) {
        if (unitTexts.at(unit).getGlobalBounds().contains(sf::Vector2f{(float) mouse.x, (float) mouse.y}) && unit !=
            unsigned(selectedUnitId)) {
            unitTexts.at(unit).setFillColor(sf::Color::Cyan);
            return 5;
        }

        if (unit != unsigned(selectedUnitId))
            unitTexts.at(unit).setFillColor(sf::Color::White);
    }

    if (saveButton->getButtonGlobalBounds().findIntersection(hitbox.getGlobalBounds())) {
        saveButton->setTextColor(sf::Color::Yellow);
        return 6;
    }
    if (backButton->getButtonGlobalBounds().findIntersection(hitbox.getGlobalBounds())) {
        backButton->setTextColor(sf::Color::Yellow);
        return 7;
    }
    saveButton->setTextColor(sf::Color::White);
    backButton->setTextColor(sf::Color::White);

    return 0;
}

int calculateUnitCost(const baseUnit unit) {
    int cost = PRICE_UNIT;
    cost += unit.vision * PRICE_VISION;
    cost += unit.health * PRICE_HEALTH;
    cost += unit.actionPoints * PRICE_ACTIONPOINT;
    cost += weaponBase::getCost(static_cast<weaponBase::weaponType>(unit.weaponId));
    return cost;
}

void Squad::recalcBuildPoints() {
    buildPoints = MAX_BUILD_POINTS;
    for (const auto &unit : unitList)
        buildPoints -= calculateUnitCost(unit);
}

void Squad::addUnit(baseUnit unit) {
    sf::Text unitText(font);
    unitText.setCharacterSize(textSize - 10);
    unitText.setFillColor(sf::Color::White);
    unitText.setPosition({50, float(unitTexts.size()) * 75 + 150});
    unitText.setString("Unit: " + std::to_string(unitTexts.size() + 1));

    unitTexts.emplace_back(unitText);
    unitList.emplace_back(unit);
    recalcBuildPoints();
}

/* Collision value list
 * 1 = pluss button
 * 2 = minus button
 * 3 = add unit button
 * 4 = remove unit button
 * 5 = unit from unit list
 * 6 = save button
 * 7 = back button
 */
void Squad::mouseClick() {
    hitbox.setSize(sf::Vector2f(1000, 55));
    hitbox.setPosition({float(mousePosition.x) - 500, float(mousePosition.y)});

    if (hitbox.getGlobalBounds().findIntersection(vision->getGlobalBounds())) {
        if (collision == 1 && buildPoints >= PRICE_VISION) {
            unitList.at(selectedUnitId).vision++;
            recalcBuildPoints();
        } else if (collision == 2 && unitList.at(selectedUnitId).vision != 1) {
            unitList.at(selectedUnitId).vision--;
            recalcBuildPoints();
        }
    } else if (hitbox.getGlobalBounds().findIntersection(health->getGlobalBounds())) {
        if (collision == 1 && buildPoints >= PRICE_HEALTH) {
            unitList.at(selectedUnitId).health += 15;
            recalcBuildPoints();
        } else if (collision == 2 && unitList.at(selectedUnitId).health != 15) {
            unitList.at(selectedUnitId).health -= 15;
            recalcBuildPoints();
        }
    } else if (hitbox.getGlobalBounds().findIntersection(actionPoints->getGlobalBounds())) {
        if (collision == 1 && buildPoints >= PRICE_ACTIONPOINT) {
            unitList.at(selectedUnitId).actionPoints++;
            recalcBuildPoints();
        } else if (collision == 2 && unitList.at(selectedUnitId).actionPoints != 1) {
            unitList.at(selectedUnitId).actionPoints--;
            recalcBuildPoints();
        }
    } else if (hitbox.getGlobalBounds().findIntersection(weapon->getGlobalBounds())) {
        auto &wId = unitList.at(selectedUnitId).weaponId;
        if (collision == 1 && wId != weaponBase::shotgunType) {
            int costDiff = weaponBase::getCost(static_cast<weaponBase::weaponType>(wId + 1))
                         - weaponBase::getCost(static_cast<weaponBase::weaponType>(wId));
            if (buildPoints >= costDiff) {
                wId++;
                recalcBuildPoints();
            }
        } else if (collision == 2 && wId != weaponBase::noType) {
            wId--;
            recalcBuildPoints();
        }
    }

    //ADD or DELETE unit down below
    else if (collision == 3 && buildPoints >= calculateUnitCost(baseUnit{}) && unitList.size() != 10)
        addUnit();

    else if (collision == 4 && unitList.size() != 1)
        removeUnit();

        //Save button pressed
    else if (collision == 6) {
        saveDrawTime += 60;
        squadIO::writeSquad(unitConversion(unitList));
    }
}

void Squad::updateValues() {
    vision->setString("Vision:        " + std::to_string(unitList.at(selectedUnitId).vision));
    health->setString("Health:        " + std::to_string(unitList.at(selectedUnitId).health));
    actionPoints->setString("AP:            " + std::to_string(unitList.at(selectedUnitId).actionPoints));
    buildPointsText->setString("Build points left: " + std::to_string(buildPoints));

    weapon->setString("Weapon: "
                      + weaponBase::getName(static_cast<weaponBase::weaponType>(unitList.at(selectedUnitId).weaponId)));
}

void Squad::selectUnit(sf::Vector2i &mouse) {
    for (unsigned long unit = 0; unit < unitList.size(); unit++) {
        if (unitTexts.at(unit).getGlobalBounds().contains(sf::Vector2f{(float) mouse.x, (float) mouse.y})) {
            unitTexts.at(unit).setFillColor(sf::Color::Red);
            selectedUnit->setString(unitTexts.at(unit).getString());
            unitTexts.at(selectedUnitId).setFillColor(sf::Color::White);
            selectedUnitId = int(unit);
            break;
        }
    }
}

void Squad::removeUnit() {
    unitList.erase(unitList.begin() + selectedUnitId);
    unitTexts.erase(unitTexts.begin() + selectedUnitId);
    recalcBuildPoints();

    if (unsigned(selectedUnitId) >= unitList.size())
        selectedUnitId--;

    unitTexts.at(selectedUnitId).setFillColor(sf::Color::Red);

    for (unsigned long unit = 0; unit < unitTexts.size(); unit++) {
        unitTexts.at(unit).setString("Unit: " + std::to_string(unit + 1));
        unitTexts.at(unit).setPosition({50, float(unit) * 75 + 150});
    }
}

std::vector<unitBase::unitClass> Squad::unitConversion(const std::vector<baseUnit> &units) {
    unitBase::unitClass convertedUnit;
    std::vector<unitBase::unitClass> convertedList;
    for (auto unit: units) {
        convertedUnit.health = unit.health;
        convertedUnit.actionPoints = unit.actionPoints;
        convertedUnit.sightRange = unit.vision;
        convertedUnit.weapon = static_cast<weaponBase::weaponType>(unit.weaponId);
        convertedList.push_back(convertedUnit);
    }
    return convertedList;
}

void Squad::getSquad(const std::vector<unitBase::unitClass> &units) {
    baseUnit convertedUnit;
    std::vector<baseUnit> convertedList;
    for (auto unit: units) {
        convertedUnit.health = unit.health;
        convertedUnit.actionPoints = unit.actionPoints;
        convertedUnit.vision = unit.sightRange;
        convertedUnit.weaponId = (int) unit.weapon;
        addUnit(convertedUnit);
    }
}
