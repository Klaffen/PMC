#ifndef DAT220_PROJECT_NETWORK_H
#define DAT220_PROJECT_NETWORK_H

#include "Client.h"
#include "ISession.h"
#include <atomic>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>

#include <SFML/Network.hpp>

class Network : public ISession {
public:
    void sendFunction();

    Network() {
        if (UdpSocket.bind(UDPPORT) != sf::Socket::Status::Done) {
            std::cout << "Failed to bind socket to UDPPort" << std::endl;
        } else {
            std::cout << "UDP socket bound to UDPPort " << UDPPORT << std::endl;
        }
        UdpSocket.setBlocking(false);
        sendThread = std::thread(&Network::sendFunction, this);
    }

    ~Network() {
        running = false;
        cvPacketQ.notify_one();
        sendThread.join();
    }

    std::deque<sf::Packet> packetQ;
    std::mutex packetQMutex;
    std::mutex clientsMutex;

    unsigned short UDPPORT = 50000;
    unsigned short TCPPORT = 51000;
    std::string gameName;

    bool isHost() const override { return _isHost; }
    void setAsHost() { _isHost = true; }
    void setAsClient() { _isHost = false; }

    sf::Packet receivePacket() override;
    void sendPacket(sf::Packet& packet) override;

    // TCP Host Specific
    sf::Packet namePacket;
    std::unique_ptr<Client> nextClient;

    void host();
    bool listen();
    void receiveClientMessage(std::vector<std::string>& list);
    std::vector<std::unique_ptr<Client>> clients;

    // TCP Client Specific
    sf::TcpSocket clientSocket;
    sf::TcpListener listener;
    sf::UdpSocket UdpSocket;

private:
    bool _isHost = true;
    std::atomic<bool> running{true};
    std::condition_variable cvPacketQ;
    std::thread sendThread;
};

#endif // DAT220_PROJECT_NETWORK_H
