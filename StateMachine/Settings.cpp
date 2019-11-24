#include <Game/GameSettings.h>
#include "Settings.h"

int Settings::enter(sf::RenderWindow &window) {

    if (!settingsInit) {

        backgroundTexture.loadFromFile("Data/Images/background_blurred.png");
        backgroundScaleX = (float)(window.getSize().x) / (float)(backgroundTexture.getSize().x);
        backgroundScaleY = (float)(window.getSize().y) / (float)(backgroundTexture.getSize().y);
        background.setScale(backgroundScaleX, backgroundScaleY);
        background.setTexture(backgroundTexture);

        keybindTexture.loadFromFile("Data/Images/keybindings.png");
        keybindScaleX = (float)(window.getSize().x) * 0.50f / (float)(keybindTexture.getSize().x);
        keybindScaleY = (float)(window.getSize().y) * 0.50f / (float)(keybindTexture.getSize().y);
        keybind.setScale(keybindScaleX, keybindScaleY);
        keybind.setTexture(keybindTexture);
        keybind.setPosition(window.getSize().x / 2.0f - keybind.getGlobalBounds().width / 2.0f, window.getSize().y / 2.0f - textSizeLarge * 3.0f);

        VMresolutions = GameSettings::getVideoModes();
        isFullscreen = true;

        resolutionTextSize(window);

        mousePointer.setSize({1, 1});

        textLablesInit(window);
        buttonInit(window);
    }

    settingsInit = true;
    process(window);

    return currentScreenState;
}

void Settings::process(sf::RenderWindow &window) {
    while (window.isOpen()) {

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                window.close();
                currentScreenState = screenState::TERMINATE;
                return;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                currentScreenState = screenState::MENU;
                return;
            }

            if (mouseInput(mousePointer, window)) return;
        }


        window.clear(sf::Color::Black);
        window.draw(background);
        window.draw(keybind);

        window.draw(fullscreen);
        window.draw(toggleWindowedFullscreen);
        window.draw(resolution);
        window.draw(resNumbers);
        window.draw(sound);
        window.draw(toggleSoundOffOn);
        window.draw(keybinds);
        window.draw(lineKeybind);

        if (resolutionChanged){
            resolutionTextSize(window);
            updateTextPosition(window);
            resolutionChanged = false;
        }

        fullscreenLEFT->draw(window);
        fullscreenRIGHT->draw(window);

        resolutionLEFT->draw(window);
        resolutionRIGHT->draw(window);

        soundLEFT->draw(window);
        soundRIGHT->draw(window);

        back->draw(window);

        window.display();
    }
}

bool Settings::mouseInput(sf::RectangleShape &mousePointer, sf::RenderWindow &window) {
    mousePointer.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

    if (mousePointer.getGlobalBounds().intersects(fullscreenLEFT->getButtonGlobalBounds())){
        fullscreenLEFT->setTextHighlight(true);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            std::string toggleText = toggleWindowedFullscreen.getString();
            if (toggleText == "FULLSCREEN"){
                toggleWindowedFullscreen.setString("WINDOWED");
                GameSettings::setWindowedMode(window);
                isFullscreen = false;
            } else{
                toggleWindowedFullscreen.setString("FULLSCREEN");
                GameSettings::setFullscreenMode(window);
                isFullscreen = true;
            }
        }
    }
    else {
        fullscreenLEFT->setTextHighlight(false);
    }

    if (mousePointer.getGlobalBounds().intersects(fullscreenRIGHT->getButtonGlobalBounds())){
        fullscreenRIGHT->setTextHighlight(true);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            std::string toggleText = toggleWindowedFullscreen.getString();
            if (toggleText == "FULLSCREEN"){
                toggleWindowedFullscreen.setString("WINDOWED");
                GameSettings::setWindowedMode(window);
                isFullscreen = false;
            } else{
                toggleWindowedFullscreen.setString("FULLSCREEN");
                GameSettings::setFullscreenMode(window);
                isFullscreen = true;
            }
        }
    }
    else {
        fullscreenRIGHT->setTextHighlight(false);
    }

    if (mousePointer.getGlobalBounds().intersects(resolutionLEFT->getButtonGlobalBounds())){
        resolutionLEFT->setTextHighlight(true);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            bool changeRes = GameSettings::setResolution(window, -1, isFullscreen);
            if (changeRes){
                std::string resNumText = std::to_string(window.getSize().x) + " x " + std::to_string(window.getSize().y);
                resNumbers.setString(resNumText);
                resolutionChanged = true;
            }
        }
    }
    else {
        resolutionLEFT->setTextHighlight(false);
    }

    if (mousePointer.getGlobalBounds().intersects(resolutionRIGHT->getButtonGlobalBounds())){
        resolutionRIGHT->setTextHighlight(true);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            bool changeRes = GameSettings::setResolution(window, +1, isFullscreen);
            if (changeRes){
                std::string resNumText = std::to_string(window.getSize().x) + " x " + std::to_string(window.getSize().y);
                resNumbers.setString(resNumText);
                resolutionChanged = true;
            }
        }
    }
    else {
        resolutionRIGHT->setTextHighlight(false);
    }

    if (mousePointer.getGlobalBounds().intersects(soundLEFT->getButtonGlobalBounds())){
        soundLEFT->setTextHighlight(true);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            std::string toggleText = toggleSoundOffOn.getString();
            if (toggleText == "ON"){
                toggleSoundOffOn.setString("OFF");
                GameSettings::muteSound();
            } else{
                toggleSoundOffOn.setString("ON");
                GameSettings::unmuteSound();
            }
        }
    }
    else {
        soundLEFT->setTextHighlight(false);
    }

    if (mousePointer.getGlobalBounds().intersects(soundRIGHT->getButtonGlobalBounds())){
        soundRIGHT->setTextHighlight(true);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            std::string toggleText = toggleSoundOffOn.getString();
            if (toggleText == "ON"){
                toggleSoundOffOn.setString("OFF");
                GameSettings::muteSound();
            } else{
                toggleSoundOffOn.setString("ON");
                GameSettings::unmuteSound();
            }
        }
    }
    else {
        soundRIGHT->setTextHighlight(false);
    }

    if (mousePointer.getGlobalBounds().intersects(back->getButtonGlobalBounds())){
        back->setTextHighlight(true);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            currentScreenState = screenState::MENU;
            return true;
        }
    }
    else {
        back->setTextHighlight(false);
    }

    return false;
}

void Settings::textLablesInit(sf::RenderWindow &window) {
    font.loadFromFile("Data/Fonts/neuropol.ttf");

    fullscreen = sf::Text("WINDOW MODE", font);
    fullscreen.setCharacterSize(textSizeLarge);
    fullscreen.setFillColor(sf::Color::White);
    fullscreen.setPosition({window.getSize().x / 20.0f, window.getSize().y / 20.0f});

    toggleWindowedFullscreen = sf::Text("FULLSCREEN", font);
    toggleWindowedFullscreen.setCharacterSize(textSizeLarge);
    toggleWindowedFullscreen.setFillColor(sf::Color::White);
    toggleWindowedFullscreen.setPosition(window.getSize().x / 2.0f, fullscreen.getPosition().y);

    resolution = sf::Text("RESOLUTION", font);
    resolution.setCharacterSize(textSizeLarge);
    resolution.setFillColor(sf::Color::White);
    resolution.setPosition({window.getSize().x / 20.0f, fullscreen.getPosition().y + (textSizeLarge * textSpaceFactor)});

    std::string resNumText = std::to_string(VMresolutions.at(0).width) + " x " + std::to_string(VMresolutions.at(0).height);
    resNumbers = sf::Text(resNumText, font);
    resNumbers.setCharacterSize(textSizeLarge);
    resNumbers.setFillColor(sf::Color::White);
    resNumbers.setPosition(window.getSize().x / 2.0f, resolution.getPosition().y);

    sound = sf::Text("SOUND", font);
    sound.setCharacterSize(textSizeLarge);
    sound.setFillColor(sf::Color::White);
    sound.setPosition({window.getSize().x / 20.0f, resolution.getPosition().y + (textSizeLarge * textSpaceFactor)});

    toggleSoundOffOn = sf::Text("ON", font);
    toggleSoundOffOn.setCharacterSize(textSizeLarge);
    toggleSoundOffOn.setFillColor(sf::Color::White);
    toggleSoundOffOn.setPosition(window.getSize().x / 2.0f, sound.getPosition().y);

    keybinds = sf::Text("KEYBINDS", font);
    keybinds.setCharacterSize(textSizeLarge);
    keybinds.setFillColor(sf::Color::White);
    keybinds.setPosition({window.getSize().x / 20.0f, sound.getPosition().y + (textSizeLarge * textSpaceFactor)});

    lineKeybind.setSize(sf::Vector2f(window.getSize().x - (window.getSize().x / 10.0f), 3));
    lineKeybind.setFillColor(sf::Color::White);
    lineKeybind.setPosition({window.getSize().x / 20.0f, keybinds.getPosition().y + (textSizeLarge * 1.25f)});
}

void Settings::buttonInit(sf::RenderWindow &window) {
    std::string font = "Data/Fonts/neuropol.ttf";
    fullscreenLEFT = std::make_shared<Button>();
    fullscreenLEFT->setFont(font);
    fullscreenLEFT->setText("<");
    fullscreenLEFT->setTextSize(textSizeLarge);
    fullscreenLEFT->setTextColor(sf::Color::White);
    fullscreenLEFT->setTextHighlightColor(sf::Color::Cyan);
    fullscreenLEFT->setPosition({toggleWindowedFullscreen.getPosition().x - textSizeLarge, fullscreen.getPosition().y + 23});
    fullscreenLEFT->setOnlyText(true);

    fullscreenRIGHT = std::make_shared<Button>();
    fullscreenRIGHT->setFont(font);
    fullscreenRIGHT->setText(">");
    fullscreenRIGHT->setTextSize(textSizeLarge);
    fullscreenRIGHT->setTextColor(sf::Color::White);
    fullscreenRIGHT->setTextHighlightColor(sf::Color::Cyan);
    fullscreenRIGHT->setPosition({toggleWindowedFullscreen.getPosition().x + toggleWindowedFullscreen.getLocalBounds().width + textSizeLarge, fullscreen.getPosition().y + 23});
    fullscreenRIGHT->setOnlyText(true);

    resolutionLEFT = std::make_shared<Button>();
    resolutionLEFT->setFont(font);
    resolutionLEFT->setText("<");
    resolutionLEFT->setTextSize(textSizeLarge);
    resolutionLEFT->setTextColor(sf::Color::White);
    resolutionLEFT->setTextHighlightColor(sf::Color::Cyan);
    resolutionLEFT->setPosition({resNumbers.getPosition().x - textSizeLarge, resolution.getPosition().y + 23});
    resolutionLEFT->setOnlyText(true);

    resolutionRIGHT = std::make_shared<Button>();
    resolutionRIGHT->setFont(font);
    resolutionRIGHT->setText(">");
    resolutionRIGHT->setTextSize(textSizeLarge);
    resolutionRIGHT->setTextColor(sf::Color::White);
    resolutionRIGHT->setTextHighlightColor(sf::Color::Cyan);
    resolutionRIGHT->setPosition({resNumbers.getPosition().x + resNumbers.getLocalBounds().width + textSizeLarge, resolution.getPosition().y + 23});
    resolutionRIGHT->setOnlyText(true);

    soundLEFT = std::make_shared<Button>();
    soundLEFT->setFont(font);
    soundLEFT->setText("<");
    soundLEFT->setTextSize(textSizeLarge);
    soundLEFT->setTextColor(sf::Color::White);
    soundLEFT->setTextHighlightColor(sf::Color::Cyan);
    soundLEFT->setPosition({toggleSoundOffOn.getPosition().x - textSizeLarge, sound.getPosition().y + 23});
    soundLEFT->setOnlyText(true);

    soundRIGHT = std::make_shared<Button>();
    soundRIGHT->setFont(font);
    soundRIGHT->setText(">");
    soundRIGHT->setTextSize(textSizeLarge);
    soundRIGHT->setTextColor(sf::Color::White);
    soundRIGHT->setTextHighlightColor(sf::Color::Cyan);
    soundRIGHT->setPosition({toggleSoundOffOn.getPosition().x + toggleSoundOffOn.getLocalBounds().width + textSizeLarge, sound.getPosition().y + 23});
    soundRIGHT->setOnlyText(true);

    back = std::make_shared<Button>();
    back->setFont(font);
    back->setText("BACK");
    back->setTextSize(textSizeLarge);
    back->setTextColor(sf::Color::White);
    back->setTextHighlightColor(sf::Color::Cyan);
    back->setPosition({window.getSize().x / 20.0f, window.getSize().y - (window.getSize().y / 10.0f)});
    back->setOnlyText(true);
}

void Settings::updateTextPosition(sf::RenderWindow &window) {
    fullscreen.setCharacterSize(textSizeLarge);
    toggleWindowedFullscreen.setCharacterSize(textSizeLarge);

    resolution.setCharacterSize(textSizeLarge);
    resNumbers.setCharacterSize(textSizeLarge);

    sound.setCharacterSize(textSizeLarge);
    toggleSoundOffOn.setCharacterSize(textSizeLarge);
    keybinds.setCharacterSize(textSizeLarge);

    //Image
    keybindScaleX = (float)(window.getSize().x) * 0.50f / (float)(keybindTexture.getSize().x);
    keybindScaleY = (float)(window.getSize().y) * 0.50f / (float)(keybindTexture.getSize().y);

    //Text
    fullscreen.setPosition({window.getSize().x / 20.0f, window.getSize().y / 20.0f});
    toggleWindowedFullscreen.setPosition(window.getSize().x / 2, fullscreen.getPosition().y);
    resolution.setPosition({window.getSize().x / 20.0f, fullscreen.getPosition().y + (textSizeLarge * textSpaceFactor)});
    resNumbers.setPosition(window.getSize().x / 2.0f, resolution.getPosition().y);
    sound.setPosition({window.getSize().x / 20.0f, resolution.getPosition().y + (textSizeLarge * textSpaceFactor)});
    toggleSoundOffOn.setPosition(window.getSize().x / 2.0f, sound.getPosition().y);
    keybinds.setPosition({window.getSize().x / 20.0f, sound.getPosition().y + (textSizeLarge * textSpaceFactor)});
    lineKeybind.setPosition({window.getSize().x / 20.0f, keybinds.getPosition().y + (textSizeLarge * 1.25f)});
    keybind.setPosition(window.getSize().x / 2.0f - keybind.getGlobalBounds().width / 2.0f, window.getSize().y / 2.0f - textSizeLarge * 2.0f);

    //Buttons
    fullscreenLEFT->setPosition({toggleWindowedFullscreen.getPosition().x - textSizeLarge, fullscreen.getPosition().y + 23});
    fullscreenRIGHT->setPosition({toggleWindowedFullscreen.getPosition().x + toggleWindowedFullscreen.getLocalBounds().width + textSizeLarge, fullscreen.getPosition().y + 23});

    resolutionLEFT->setPosition({resNumbers.getPosition().x - textSizeLarge, resolution.getPosition().y + 23});
    resolutionRIGHT->setPosition({resNumbers.getPosition().x + resNumbers.getLocalBounds().width + textSizeLarge, resolution.getPosition().y + 23});

    soundLEFT->setPosition({toggleSoundOffOn.getPosition().x - textSizeLarge, sound.getPosition().y + 23});
    soundRIGHT->setPosition({toggleSoundOffOn.getPosition().x + toggleSoundOffOn.getLocalBounds().width + textSizeLarge, sound.getPosition().y + 23});

    back->setPosition({window.getSize().x / 20.0f, window.getSize().y - (window.getSize().y / 10.0f)});
}

void Settings::resolutionTextSize(sf::RenderWindow &window) {

    if (window.getSize().y <= 900 || window.getSize().x <= 1300){
        textSizeLarge = 44;
        textSizeMedium = 32;
        textSizeSmall = 20;

        keybindScaleX = (float)(window.getSize().x) * 0.40f / (float)(keybindTexture.getSize().x);
        keybindScaleY = (float)(window.getSize().y) * 0.40f / (float)(keybindTexture.getSize().y);
        keybind.setScale(keybindScaleX, keybindScaleY);

    } else{
        textSizeLarge = 56;
        textSizeMedium = 44;
        textSizeSmall = 32;

        keybindScaleX = (float)(window.getSize().x) * 0.50f / (float)(keybindTexture.getSize().x);
        keybindScaleY = (float)(window.getSize().y) * 0.50f / (float)(keybindTexture.getSize().y);
        keybind.setScale(keybindScaleX, keybindScaleY);

    }
}


