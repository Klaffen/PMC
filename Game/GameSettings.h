#ifndef DAT220_PROJECT_GAMESETTINGS_H
#define DAT220_PROJECT_GAMESETTINGS_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio/Music.hpp>
#include <Game/Objects/Units/UnitBase.h>

class GameSettings {

public:
    /**
     * Set window to fullscreen
     * @param window The window to change
     */
    static void setFullscreenMode(sf::RenderWindow &window);
    /**
     * Set window to windowed mode
     * @param window window to change
     */
    static void setWindowedMode(sf::RenderWindow &window);
    /**
     * Get the supported resolution for the current system
     * @return A vector of supported resolutions
     */
    static const std::vector<sf::VideoMode> & getVideoModes();
    /**
     * Set resolution of the window
     * @param window window to change
     * @param change Increase or decrease resolution
     * @param isFullscren If the window is fullscreen or not
     * @return If changing resolution is successful or not
     */
    static bool setResolution(sf::RenderWindow &window, int change, bool isFullscren);
    /**
     * Plays music
     */
    static void playMusic();
    /**
     * Play sound on a specific unit
     * @param unit the unit that plays the sound
     */
    static void playUnitSound(unitBase &unit);
    /**
     * Mutes all sounds
     */
    static void muteSound();
    /**
     * Unmutes all sounds
     */
    static void unmuteSound();

private:

};

#endif //DAT220_PROJECT_GAMESETTINGS_H
