#ifndef DAT220_PROJECT_GAME_STATE_STATE_H
#define DAT220_PROJECT_GAME_STATE_STATE_H

#include <SFML/Graphics.hpp>
#include <memory>

class State {

public:

    enum screenState {MENU, GAME, LOBBY, SETTINGS, SQUAD, TERMINATE};

    virtual int enter(sf::RenderWindow &window) = 0;
    virtual void process(sf::RenderWindow &window) = 0;

protected:
    screenState currentScreenState;
};

typedef std::shared_ptr<State> StatePtr;

#endif //DAT220_PROJECT_GAME_STATE_STATE_H
