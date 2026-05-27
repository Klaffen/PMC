#include "Network.h"

void Network::host() {
    if (listener.listen(TCPPORT) != sf::Socket::Status::Done) {
        std::cout << "Failed to bind TCP listener to TCPPort" << std::endl;
    } else {
        std::cout << "TCP listener bound to port " << TCPPORT << std::endl;
    }
    listener.setBlocking(false);
    nextClient = std::make_unique<Client>();
}

bool Network::listen() {
    (void) UdpSocket.send(namePacket, sf::IpAddress::Broadcast, UDPPORT);

    if (listener.accept(nextClient->socket) == sf::Socket::Status::Done) {
        sf::Packet packet;
        (void) nextClient->socket.receive(packet);
        packet >> nextClient->playerName;
        nextClient->socket.setBlocking(false);
        std::cout << "\nAdded: " << nextClient->playerName << std::endl;
        {
            std::lock_guard lock(clientsMutex);
            clients.emplace(clients.begin(), std::move(nextClient));
        }
        nextClient = std::make_unique<Client>();

        std::cout << "List of clients" << std::endl;
        {
            std::lock_guard lock(clientsMutex);
            for (auto& client : clients) {
                std::cout << client->playerName << std::endl;
            }
        }

        return true;
    }
    return false;
}

void Network::receiveClientMessage(std::vector<std::string>& list) {
    sf::Packet packet;
    std::string message, combinedMessage;
    int type;

    std::lock_guard lock(clientsMutex);
    for (auto& client : clients) {
        type = 0;
        (void) client->socket.receive(packet);
        packet >> type;
        if (type == 1337) {
            packet >> message;
            combinedMessage = client->playerName + ": " + message;
            list.push_back(combinedMessage);
            packet.clear();
            packet << 101 << message << client->playerName;
            std::cout << "Received: " << message << " From: " << client->playerName << std::endl;
            for (auto& otherClient : clients) {
                if (otherClient->playerName != client->playerName) {
                    std::cout << "Sending message to: " << otherClient->playerName << std::endl;
                    (void) otherClient->socket.send(packet);
                }
            }
            packet.clear();
        }
    }
}

sf::Packet Network::receivePacket() {
    sf::Packet packet;
    if (HOST) {
        std::lock_guard lock(clientsMutex);
        for (auto& client : clients) {
            (void) client->socket.receive(packet);
            if (packet.getDataSize() > 0) {
                std::cout << "Packet received from: " << client->playerName << std::endl;

                for (auto& otherClient : clients) {
                    if (otherClient->playerName != client->playerName) {
                        std::cout << "Sending packet to: " << otherClient->playerName << std::endl;
                        (void) otherClient->socket.send(packet);
                    }
                }
                return packet;
            }
        }
    } else {
        (void) clientSocket.receive(packet);
        if (packet.getDataSize() > 0) {
            std::cout << "Received packet from host" << std::endl;
        }
        return packet;
    }
    return packet;
}

void Network::sendPacket(sf::Packet& packet) {
    {
        std::lock_guard lock(packetQMutex);
        packetQ.emplace_back(packet);
    }
    cvPacketQ.notify_one();
}

void Network::sendFunction() {
    while (true) {
        sf::Packet payload;
        {
            std::unique_lock lock(packetQMutex);
            cvPacketQ.wait(lock, [this] { return !packetQ.empty() || !running; });
            if (!running && packetQ.empty()) break;
            payload = std::move(packetQ.front());
            packetQ.pop_front();
        }
        if (HOST) {
            std::lock_guard lock(clientsMutex);
            for (auto& client : clients) {
                sf::Socket::Status status = client->socket.send(payload);
                if (status == sf::Socket::Status::Done) {
                    std::cout << "Packet sent to player: " << client->playerName << std::endl;
                } else {
                    std::cout << "Failed to send packet to player: " << client->playerName << std::endl;
                }
            }
        } else {
            if (clientSocket.send(payload) == sf::Socket::Status::Done) {
                std::cout << "Packet sent to host" << std::endl;
            } else {
                std::cout << "Failed to send packet to host" << std::endl;
            }
        }
    }
}
