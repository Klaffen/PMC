#include <Game/GameSettings.h>
#include "Menu.h"

int Menu::enter(sf::RenderWindow &window) {
    //Reset view
    menuView = window.getView();
    menuView.setSize(window.getSize().x, window.getSize().y);
    menuView.setCenter(window.getSize().x/2, window.getSize().y/2);
    menuView.setRotation(0);
    window.setView(menuView);


    backgroundTexture.loadFromFile("Data/Images/background_title.png");
    backgroundScaleX = (float)(window.getSize().x) / (float)(backgroundTexture.getSize().x);
    backgroundScaleY = (float)(window.getSize().y) / (float)(backgroundTexture.getSize().y);
    background.setScale(backgroundScaleX, backgroundScaleY);
    background.setTexture(backgroundTexture);

    mousePointer.setSize({1, 1});

    buttonInit(window);

    if (!menuInit) {
        GameSettings::playMusic();
    }

    menuInit = true;

    process(window);
    return currentScreenState;
}

void Menu::process(sf::RenderWindow &window) {

    while (window.isOpen()) {
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                window.close();
                currentScreenState = screenState::TERMINATE;
                return;
            }
        }

        if (mouseInput(mousePointer, window)) return;

        window.clear(sf::Color(22, 21, 64));
        window.draw(background);

        playButton->draw(window);
        squadButton->draw(window);
        settingsButton->draw(window);
        exitButton->draw(window);

        window.display();
    }
}

void Menu::buttonInit(const sf::RenderWindow &window) {
    playButton = std::make_shared<Button>();
    playButton->setFont("Data/Fonts/ethnocentric rg.ttf");
    playButton->setText("PLAY");
    playButton->setTextSize(78);
    playButton->setTextColor(sf::Color(253, 216, 53));
    playButton->setTextOutlineColor(sf::Color(213, 0, 0));
    playButton->setTextOutlineSize(2);
    playButton->setPosition({(window.getSize().x / 16.0f),
                             window.getSize().y / 2.0f});
    playButton->setOnlyText(true);

    squadButton = std::make_shared<Button>();
    squadButton->setFont("Data/Fonts/ethnocentric rg.ttf");
    squadButton->setText("SQUAD BUILDER");
    squadButton->setTextSize(78);
    squadButton->setTextColor(sf::Color(253, 216, 53));
    squadButton->setTextOutlineColor(sf::Color(213, 0, 0));
    squadButton->setTextOutlineSize(2);
    squadButton->setPosition({(window.getSize().x / 16.0f),
                             playButton->getPosition().y + squadButton->getButtonGlobalBounds().height * 1.8f});
    squadButton->setOnlyText(true);

    settingsButton = std::make_shared<Button>();
    settingsButton->setFont("Data/Fonts/ethnocentric rg.ttf");
    settingsButton->setText("OPTIONS");
    settingsButton->setTextSize(78);
    settingsButton->setTextColor(sf::Color(253, 216, 53));
    settingsButton->setTextOutlineColor(sf::Color(213, 0, 0));
    settingsButton->setTextOutlineSize(2);
    settingsButton->setPosition({(window.getSize().x / 16.0f),
                                 (squadButton->getPosition().y + settingsButton->getButtonGlobalBounds().height * 1.8f)});
    settingsButton->setOnlyText(true);

    exitButton = std::make_shared<Button>();
    exitButton->setFont("Data/Fonts/ethnocentric rg.ttf");
    exitButton->setText("EXIT");
    exitButton->setTextSize(78);
    exitButton->setTextColor(sf::Color(253, 216, 53));
    exitButton->setTextOutlineColor(sf::Color(213, 0, 0));
    exitButton->setTextOutlineSize(2);
    exitButton->setPosition({(window.getSize().x / 16.0f),
                             (settingsButton->getPosition().y + exitButton->getButtonGlobalBounds().height * 1.8f)});
    exitButton->setOnlyText(true);
}

bool Menu::mouseInput(sf::RectangleShape &mousePointer, const sf::RenderWindow &window) {
    mousePointer.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
    if (mousePointer.getGlobalBounds().intersects(playButton->getButtonGlobalBounds())) {
        playButton->setButtonHighlight(true);
        playButton->setTextHighlight(true);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            currentScreenState = screenState::LOBBY;
            return true;
        }
    }
    else {
        playButton->setButtonHighlight(false);
        playButton->setTextHighlight(false);
    }

    mousePointer.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
    if (mousePointer.getGlobalBounds().intersects(squadButton->getButtonGlobalBounds())) {
        squadButton->setButtonHighlight(true);
        squadButton->setTextHighlight(true);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            currentScreenState = screenState::SQUAD;
            return true;
        }
    }
    else {
        squadButton->setButtonHighlight(false);
        squadButton->setTextHighlight(false);
    }

    if (mousePointer.getGlobalBounds().intersects(settingsButton->getButtonGlobalBounds())) {
        settingsButton->setButtonHighlight(true);
        settingsButton->setTextHighlight(true);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            currentScreenState = screenState::SETTINGS;
            return true;
        }
    }
    else {
        settingsButton->setButtonHighlight(false);
        settingsButton->setTextHighlight(false);
    }

    if (mousePointer.getGlobalBounds().intersects(exitButton->getButtonGlobalBounds())) {
        exitButton->setButtonHighlight(true);
        exitButton->setTextHighlight(true);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            currentScreenState = screenState::TERMINATE;
            return true;
        }
    }
    else {
        exitButton->setButtonHighlight(false);
        exitButton->setTextHighlight(false);
    }

    return false;
}
