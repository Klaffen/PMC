
#include "Game/GameSettings.h"
#include "StateMachine/Game.h"
#include "StateMachine/Lobby.h"
#include "StateMachine/Menu.h"
#include "StateMachine/Settings.h"
#include "StateMachine/Squad.h"
int main() {
    sf::ContextSettings windowSettings;
    windowSettings.antiAliasingLevel = 15;

    std::vector<sf::VideoMode> resolution = GameSettings::getVideoModes();

    sf::RenderWindow window(resolution.at(0), "Shape Shooter", sf::State::Fullscreen, windowSettings);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    auto network = std::make_unique<Network>();
    MenuPtr menu = std::make_shared<Menu>();
    GamePtr game = std::make_shared<Game>(network.get());
    LobbyPtr lobby = std::make_shared<Lobby>(network.get(), &game->matchState);
    SettingsPtr settings = std::make_shared<Settings>();
    SquadPtr squad = std::make_shared<Squad>();

    std::vector<StatePtr> states;

    int currentState = State::screenState::MENU; // start with menu

    states.push_back(menu);
    states.push_back(game);
    states.push_back(lobby);
    states.push_back(settings);
    states.push_back(squad);

    // game loop
    while (currentState != State::screenState::TERMINATE) {
        currentState = states[currentState]->enter(window);
    }

    return 0;
}
