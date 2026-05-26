#ifndef DAT220_PROJECT_NETWORK_H
#define DAT220_PROJECT_NETWORK_H

#include "Client.h"
#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Network.hpp>

class Network {
public:
    /**
     * Function used by network thread to send packets
     * @return Returns nothing
     */
    void sendFunction();
    // Constructor that binds the UDP socket to the UDP port
    Network() {
        if (UdpSocket.bind(UDPPORT) != sf::Socket::Status::Done) {
            std::cout << "Failed to bind socket to UDPPort" << std::endl;
        } else {
            std::cout << "UDP socket bound to UDPPort " << UDPPORT << std::endl;
        }
        sendThread = std::thread(&Network::sendFunction, this);
    }

    ~Network() {
        running = false;
        sendThread.join();
    }

    std::vector<sf::Packet> packetQ;

    std::mutex packetQMutex;

    sf::Packet payload;

    // TCP Common
    unsigned short UDPPORT = 50000;
    unsigned short TCPPORT = 51000;
    std::string gameName;
    bool HOST = true;
    /**
     * Checks if there is an incomming packet
     * @param blocking Used to specify if the thread should be set as blocked untill packet received
     * @return Returns either an incomming packet or an empty one if nothing was received
     */
    sf::Packet receivePacket(bool blocking);

    /**
     * Adds packet to send vector
     * @param packet Reference packet that is to be sent
     * @param blocking Unused. Only there for compatibility reasons
     * @return Returns nothing
     */
    void sendPacket(sf::Packet& packet, bool blocking);

    // TCP Host Specific
    sf::Packet namePacket;
    Client* nextClient;

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
     * @param blocking Used to specify if the thread should be blocked untill a message is received
     * @param list reference to the list of messages
     * @return Returns nothing
     */
    void receiveClientMessage(bool blocking, std::vector<std::string>& list);
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
    std::thread sendThread;
};

#endif
