#include "GameSettings.h"

static bool mute = false;
static sf::Music music;

void GameSettings::setFullscreenMode(sf::RenderWindow &window) {
    unsigned int width = window.getSize().x;
    unsigned int height = window.getSize().y;

    window.create(sf::VideoMode(width, height), "Shape Shooter", sf::Style::Fullscreen);
}

void GameSettings::setWindowedMode(sf::RenderWindow &window) {
    unsigned int width = window.getSize().x;
    unsigned int height = window.getSize().y;

    window.create(sf::VideoMode(width, height), "Shape Shooter", sf::Style::Default);
}

const std::vector<sf::VideoMode> & GameSettings::getVideoModes() {
    return sf::VideoMode::getFullscreenModes();
}

bool GameSettings::setResolution(sf::RenderWindow &window, int change, bool isFullscreen) {
    unsigned int width = window.getSize().x;
    unsigned int height = window.getSize().y;

    std::vector<sf::VideoMode> resolutions = getVideoModes();
    int index = 0;
    for (auto &resolution : resolutions) {
        if (width == resolution.width && height == resolution.height) break;
        ++index;
    }


    unsigned int resIndex = index + change;
    if (resIndex>= 0 || resIndex <= (resolutions.size() - 1)){
        if (isFullscreen) {
            window.create(resolutions.at(resIndex), "Shape Shooter", sf::Style::Fullscreen);
            return true;
        }
        else {
            window.create(resolutions.at(resIndex), "Shape Shooter");
            return true;
        }
    }

    return false;
}

void GameSettings::playMusic() {
    if (!mute) {
        music.openFromFile("Data/Audio/through space.ogg");
        music.setVolume(50);
        music.setLoop(true);
        music.play();
    }
}

void GameSettings::playUnitSound(unitBase &unit) {
    if (!mute){
        unit.getWeapon()->sound.play();
    }
}

void GameSettings::muteSound() {
    mute = true;
    music.pause();
}

void GameSettings::unmuteSound() {
    mute = false;
    music.play();
}
