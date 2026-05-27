#ifndef DAT220_PROJECT_MATCHSTATE_H
#define DAT220_PROJECT_MATCHSTATE_H

#include <string>
#include <SFML/Graphics/Color.hpp>

struct MatchState {
    bool turn        = true;
    bool alive       = true;
    int playerNumber = 0;
    std::string titleString;
    sf::Color titleColor = sf::Color::White;
};

#endif // DAT220_PROJECT_MATCHSTATE_H
