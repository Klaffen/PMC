#ifndef DAT220_PROJECT_CLIENT_H
#define DAT220_PROJECT_CLIENT_H

#include <string>
#include <SFML/Network.hpp>


class Client {
public:
    enum playerStatus {turn = 0, opponentsTurn = 2, defeat = 3, victory = 4};
    std::string playerName;
    sf::TcpSocket socket;
    playerStatus status = opponentsTurn;
    bool alive = true;
};

#endif