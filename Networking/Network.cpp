#include "Network.h"

void Network::host() {
    // bind the listener to the tcp port
    if (listener.listen(TCPPORT) != sf::Socket::Done)
        std::cout << "Failed to bind TCP listener to TCPPort" << std::endl;
    else
        std::cout << "TCP listener bound to port " << TCPPORT << std::endl;

    nextClient = new Client();
}

bool Network::listen() {
    UdpSocket.setBlocking(false);

    UdpSocket.send(namePacket, sf::IpAddress::Broadcast, UDPPORT);

    listener.setBlocking(false);
    if (listener.accept(nextClient->socket) == sf::Socket::Done) {
        sf::Packet packet;
        nextClient->socket.receive(packet);
        packet >> nextClient->playerName;
        clients.emplace(clients.begin(), nextClient);
        std::cout << "\nAdded: " << nextClient->playerName << std::endl;
        nextClient = new Client();

        std::cout << "List of clients" << std::endl;
        for (auto &client : clients)
            std::cout << client->playerName << std::endl;

        return true;
    }
    return false;
}

void Network::receiveClientMessage(bool blocking, std::vector<std::string> &list) {
    sf::Packet packet;
    std::string message, combinedMessage;
    int type;

    for (auto &client : clients) {
        type = 0;
        client->socket.setBlocking(blocking);
        client->socket.receive(packet);
        packet >> type;
        if (type == 1337) {
            packet >> message;
            combinedMessage = client->playerName + ": " + message;
            list.push_back(combinedMessage);
            packet.clear();
            packet << 101 << message << client->playerName;
            std::cout << "Received: " << message << " From: " << client->playerName << std::endl;
            for (auto &otherClient : clients) {
                if (otherClient->playerName != client->playerName) {
                    std::cout << "Sending message to: " << otherClient->playerName << std::endl;
                    otherClient->socket.send(packet);
                }
            }
            packet.clear();
        }
    }
}

sf::Packet Network::receivePacket(bool blocking) {
    sf::Packet packet;
    if (HOST) {
        for (auto &client : clients) {
            client->socket.setBlocking(false);
            client->socket.receive(packet);
            if (packet.getData() != NULL) {
                std::cout << "Packet received from: " << client->playerName << std::endl;

                for (auto &otherClient : clients) {
                    if (otherClient->playerName != client->playerName) {
                        std::cout << "Sending packet to: " << otherClient->playerName << std::endl;
                        otherClient->socket.send(packet);
                    }
                }
                return packet;
            }
        }
    }
    else {
        clientSocket.setBlocking(false);
        clientSocket.receive(packet);
        if (!packet.endOfPacket())
            std::cout << "Received packet from host" << std::endl;
        return packet;
    }
    return packet;
}

void Network::sendPacket(sf::Packet &packet, bool blocking) {
    packetQMutex.lock();
    packetQ.emplace_back(packet);
    packetQMutex.unlock();
}

void Network::sendFunction() {
    while (1) {
        if (!packetQ.empty()) {
            packetQMutex.lock();
            payload = packetQ.at(0);
            packetQ.erase(packetQ.begin());
            packetQMutex.unlock();
            if (HOST) {
                sf::Socket::Status  status;
                for (auto &client : clients) {
                    client->socket.setBlocking(true);
                    status = client->socket.send(payload);
                    if (status == sf::Socket::Done)
                        std::cout << "Packet sent to player: " << client->playerName << std::endl;
                    else
                        std::cout << "Failed to send packet to player: " << client->playerName << std::endl;
                }
            }
            else {
                clientSocket.setBlocking(true);
                if (clientSocket.send(payload) == sf::Socket::Done)
                    std::cout << "Packet sent to host" << std::endl;
                else
                    std::cout << "Failed to send packet to host" << std::endl;
            }
        }
    }
}