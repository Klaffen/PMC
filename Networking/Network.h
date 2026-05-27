#ifndef DAT220_PROJECT_NETWORK_H
#define DAT220_PROJECT_NETWORK_H

#include "Client.h"
#include <atomic>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Network.hpp>

class Network {
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

    // TCP Common
    unsigned short UDPPORT = 50000;
    unsigned short TCPPORT = 51000;
    std::string gameName;
    bool HOST = true;
    /**
     * Checks if there is an incomming packet
     * @return Returns either an incomming packet or an empty one if nothing was received
     */
    sf::Packet receivePacket();

    /**
     * Adds packet to send vector
     * @param packet Reference packet that is to be sent
     * @return Returns nothing
     */
    void sendPacket(sf::Packet& packet);

    // TCP Host Specific
    sf::Packet namePacket;
    std::unique_ptr<Client> nextClient;

    /**
     * Sets up listening socket and adds a creates client object
     * @return Returns nothing
     */
    void host();

    /**
     * Listen for connecting clients
     * @return Returns nothing
     */
    bool listen();

    /**
     * Checks for incomming text messages from clients
     * @param list reference to the list of messages
     * @return Returns nothing
     */
    void receiveClientMessage(std::vector<std::string>& list);
    std::vector<std::unique_ptr<Client>> clients;

    // TCP Client Specific
    sf::TcpSocket clientSocket; // Used by TCP clients

    sf::TcpListener listener; // Used by the TCP lobby to connect to clients
    sf::UdpSocket UdpSocket; // Used by both client and server for UDP packets

    bool turn        = true;
    bool alive       = true;
    int playerNumber = 0;
    std::string titleString;
    sf::Color titleColor = sf::Color::White;

private:
    std::atomic<bool> running{true};
    std::condition_variable cvPacketQ;
    std::thread sendThread;
};

#endif
