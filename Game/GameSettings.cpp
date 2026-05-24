#include <optional>
#include "GameSettings.h"

static bool mute = false;
static std::optional<sf::Music> music;

void GameSettings::setFullscreenMode(sf::RenderWindow &window) {
    unsigned int width = window.getSize().x;
    unsigned int height = window.getSize().y;

    window.create(sf::VideoMode({width, height}), "Shape Shooter", sf::State::Fullscreen);
}

void GameSettings::setWindowedMode(sf::RenderWindow &window) {
    unsigned int width = window.getSize().x;
    unsigned int height = window.getSize().y;

    window.create(sf::VideoMode({width, height}), "Shape Shooter");
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
        if (width == resolution.size.x && height == resolution.size.y) break;
        ++index;
    }


    unsigned int resIndex = index + change;
    if (resIndex>= 0 || resIndex <= (resolutions.size() - 1)){
        if (isFullscreen) {
            window.create(resolutions.at(resIndex), "Shape Shooter", sf::State::Fullscreen);
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
        music.emplace();
        if (music->openFromFile("Data/Audio/through space.ogg")) {
            music->setVolume(50);
            music->setLooping(true);
            music->play();
        } else {
            music.reset();
        }
    }
}

void GameSettings::playUnitSound(unitBase &unit) {
    if (!mute){
        if (unit.getWeapon()->sound)
            unit.getWeapon()->sound->play();
    }
}

void GameSettings::muteSound() {
    mute = true;
    if (music) music->pause();
}

void GameSettings::unmuteSound() {
    mute = false;
    if (music) music->play();
}
