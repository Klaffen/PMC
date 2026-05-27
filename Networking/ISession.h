#ifndef DAT220_PROJECT_ISESSION_H
#define DAT220_PROJECT_ISESSION_H

#include <SFML/Network/Packet.hpp>

class ISession {
public:
    virtual ~ISession() = default;
    virtual void sendPacket(sf::Packet& packet) = 0;
    virtual sf::Packet receivePacket()          = 0;
    virtual bool isHost() const                 = 0;
};

#endif // DAT220_PROJECT_ISESSION_H
