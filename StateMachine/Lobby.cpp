#include "Lobby.h"

#include <algorithm>
#include <cstring>
#include <stdexcept>

#define PACKET_UPDATE  100
#define PACKET_MESSAGE 101
#define PACKET_START   102

Lobby::Lobby(Network* network) {
    this->network = network;
}

int Lobby::enter(sf::RenderWindow& window) {
    if (!backgroundTexture.loadFromFile("Data/Images/background_blurred.png")) {
        throw std::runtime_error("Could not load background texture!");
    }
    backgroundScaleX = (float) (window.getSize().x) / (float) (backgroundTexture.getSize().x);
    backgroundScaleY = (float) (window.getSize().y) / (float) (backgroundTexture.getSize().y);
    background.emplace(backgroundTexture);
    background->setScale({backgroundScaleX, backgroundScaleY});

    if (!font.openFromFile("Data/Fonts/Inconsolata-Regular.ttf")) {
        throw std::runtime_error("Could not load font!");
    }

    userInput.emplace(font);
    userInput->setCharacterSize(40);
    userInput->setPosition({230, window.getView().getSize().y / 2 + 100});
    userInput->setFillColor(sf::Color::White);
    userInput->setString("");

    textElement.emplace(font);
    textElement->setCharacterSize(28);
    textElement->setFillColor(sf::Color::White);

    inputTag.emplace(font);
    inputTag->setCharacterSize(40);
    inputTag->setPosition({100, window.getView().getSize().y / 2 + 100});
    inputTag->setFillColor(sf::Color::White);
    inputTag->setString("input: ");

    chatBox.setOutlineColor(sf::Color::White);
    chatBox.setOutlineThickness(2);
    chatBox.setFillColor(sf::Color(0, 0, 0, 150));
    chatBox.setPosition({50, 50});
    chatBox.setSize(sf::Vector2f(window.getView().getSize().x - 100, window.getView().getSize().y - 100));

    titlebar.emplace(font);
    titlebar->setCharacterSize(32);
    titlebar->setFillColor(sf::Color::White);
    titlebar->setPosition({50, 10});

    hostButton = std::make_shared<Button>();
    hostButton->setFont("Data/Fonts/ethnocentric rg.ttf");
    hostButton->setText("Host");
    hostButton->setTextColor(sf::Color(253, 216, 53));
    hostButton->setTextOutlineColor(sf::Color(213, 0, 0));
    hostButton->setTextOutlineSize(2);
    hostButton->setTextSize(78);
    hostButton->setPosition(
        {window.getSize().x / 2.0f - (hostButton->getButtonLocalBounds().size.x / 2.0f), window.getSize().y / 3.0f});
    hostButton->setOnlyText(true);

    joinButton = std::make_shared<Button>();
    joinButton->setFont("Data/Fonts/ethnocentric rg.ttf");
    joinButton->setText("Join");
    joinButton->setTextColor(sf::Color(253, 216, 53));
    joinButton->setTextOutlineColor(sf::Color(213, 0, 0));
    joinButton->setTextOutlineSize(2);
    joinButton->setTextSize(78);
    joinButton->setPosition({window.getSize().x / 2.0f - (joinButton->getButtonLocalBounds().size.x / 2.0f),
        hostButton->getPosition().y + (78 * 1.5f)});
    joinButton->setOnlyText(true);

    joinManuallyButton = std::make_shared<Button>();
    joinManuallyButton->setFont("Data/Fonts/ethnocentric rg.ttf");
    joinManuallyButton->setText("Join IP");
    joinManuallyButton->setTextColor(sf::Color(253, 216, 53));
    joinManuallyButton->setTextOutlineColor(sf::Color(213, 0, 0));
    joinManuallyButton->setTextOutlineSize(2);
    joinManuallyButton->setTextSize(78);
    joinManuallyButton->setPosition(
        {window.getSize().x / 2.0f - (joinManuallyButton->getButtonLocalBounds().size.x / 2.0f),
            joinButton->getPosition().y + (78 * 1.5f)});
    joinManuallyButton->setOnlyText(true);

    gameNameText.emplace(font);
    gameNameText->setCharacterSize(52);
    gameNameText->setFillColor(sf::Color::White);
    gameNameText->setPosition(
        {window.getSize().x / 2.0f - (gameNameText->getGlobalBounds().size.x / 2.0f), window.getSize().y / 10.0f});
    gameNameText->setString("");

    startGameButton = std::make_shared<Button>();
    startGameButton->setTextColor(sf::Color::White);
    startGameButton->setButtonColor(sf::Color::Black);
    startGameButton->setPosition({window.getSize().x - (float) 300, window.getView().getSize().y - (float) 60});

    startIntervall = 0;
    endIntervall   = (int) ((window.getView().getSize().y - 50) - 60) / 50;

    lobbySetup(window);

    if (currentScreenState != screenState::TERMINATE) {
        process(window);
    }

    return currentScreenState;
}

void Lobby::lobbySetup(sf::RenderWindow& window) {
    std::string string;
    enum stages { setup, nameInput, ipInput, complete };
    stages stage = setup;
    while (window.isOpen()) {

        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                currentScreenState = screenState::TERMINATE;
                return;
            }
            if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
                if (stage != setup) {
                    if (textEntered->unicode < 128 && textEntered->unicode != 8) {
                        string += static_cast<char>(textEntered->unicode);
                        string.erase(std::remove(string.begin(), string.end(), '\n'), string.end());
                        string.erase(std::remove(string.begin(), string.end(), '\r'), string.end());
                        userInput->setString(string);
                    } else if (textEntered->unicode == 8 && !string.empty()) {
                        string.pop_back();
                        userInput->setString(string);
                    }
                }
            }

            switch (stage) {
            case complete:
                return;

            case setup:
                {
                    sf::Vector2f mousePos(sf::Mouse::getPosition(window));
                    if (hostButton->getButtonGlobalBounds().contains(mousePos)) {
                        hostButton->setTextColor(sf::Color::White);
                        joinButton->setTextColor(sf::Color(253, 216, 53));
                        joinManuallyButton->setTextColor(sf::Color(253, 216, 53));
                    } else if (joinButton->getButtonGlobalBounds().contains(mousePos)) {
                        hostButton->setTextColor(sf::Color(253, 216, 53));
                        joinButton->setTextColor(sf::Color::White);
                        joinManuallyButton->setTextColor(sf::Color(253, 216, 53));
                    } else if (joinManuallyButton->getButtonGlobalBounds().contains(mousePos)) {
                        joinButton->setTextColor(sf::Color(253, 216, 53));
                        hostButton->setTextColor(sf::Color(253, 216, 53));
                        joinManuallyButton->setTextColor(sf::Color::White);
                    } else {
                        joinButton->setTextColor(sf::Color(253, 216, 53));
                        hostButton->setTextColor(sf::Color(253, 216, 53));
                        joinManuallyButton->setTextColor(sf::Color(253, 216, 53));
                    }

                    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)
                        && hostButton->getButtonGlobalBounds().contains(mousePos)) {
                        std::cout << "Host selected" << std::endl;
                        gameNameText->setString("Enter a game name");
                        gameNameText->setPosition(
                            {window.getSize().x / 2.0f - (gameNameText->getGlobalBounds().size.x / 2.0f),
                                window.getSize().y / 10.0f});
                        network->HOST        = true;
                        network->titleString = "Your turn";
                        network->titleColor  = sf::Color::White;
                        network->host();
                        hostGlobalIP = "";
                        if (auto addr = sf::IpAddress::getLocalAddress()) {
                            hostLocalIP = addr->toString();
                        }

                        stage = nameInput;
                        inputTag->setString("Input:");
                        string = "";
                        userInput->setString(string);
                    } else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)
                               && joinManuallyButton->getButtonGlobalBounds().contains(mousePos)) {
                        stage                = ipInput;
                        network->HOST        = false;
                        network->titleString = "Opponent's turn";
                        network->titleColor  = sf::Color::Blue;
                        break;
                    } else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)
                               && joinButton->getButtonGlobalBounds().contains(mousePos)) {
                        std::cout << "Join selected" << std::endl;
                        network->HOST        = false;
                        network->titleString = "Opponent's turn";
                        network->titleColor  = sf::Color::Blue;
                        sf::Packet packet;
                        std::optional<sf::IpAddress> throwaway;
                        unsigned short senderPort = 0;
                        network->UdpSocket.setBlocking(false);
                        (void) network->UdpSocket.receive(packet, throwaway, senderPort);

                        packet >> network->gameName >> hostLocalIP >> hostGlobalIP;

                        if (!hostGlobalIP.empty()) {
                            std::cout << "Found game with name: " << network->gameName << " And IP: " << hostGlobalIP
                                      << std::endl;
                            gameNameText->setString(
                                "Found a game with name: " + network->gameName + "\nEnter your name");
                            gameNameText->setPosition(
                                {window.getSize().x / 2.0f - (gameNameText->getGlobalBounds().size.x / 2.0f),
                                    window.getSize().y / 10.0f});

                            sf::Socket::Status status = sf::Socket::Status::Error;
                            if (auto addrs = sf::Dns::resolve(hostLocalIP); addrs && !addrs->empty()) {
                                status = network->clientSocket.connect(addrs->front(), network->TCPPORT);
                            }
                            if (status != sf::Socket::Status::Done) {
                                std::cout << "Failed to establish a tcp connection using the local IP. Trying the "
                                             "global ip instead"
                                          << std::endl;
                                if (auto addrs = sf::Dns::resolve(hostGlobalIP); addrs && !addrs->empty()) {
                                    status = network->clientSocket.connect(addrs->front(), network->TCPPORT);
                                }
                                if (status != sf::Socket::Status::Done) {
                                    std::cout << "Failed to establish a tcp connection." << std::endl;
                                    break;
                                }
                            } else {
                                std::cout << "TCP Connection established. Ready to roll" << std::endl;
                            };
                            inputTag->setString("Input:");
                            string = "";
                            userInput->setString(string);
                            stage = nameInput;
                        } else {
                            gameNameText->setString("No game found");
                            gameNameText->setPosition(
                                {window.getSize().x / 2.0f - (gameNameText->getGlobalBounds().size.x / 2.0f),
                                    window.getSize().y / 10.0f});
                            std::cout << "No game found. Please try again, or host your own game." << std::endl;
                        }
                    }
                    break;
                }

            case nameInput:
                {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
                        if (network->HOST && !string.empty()) {
                            network->gameName = string;
                            network->namePacket << string << hostLocalIP << hostGlobalIP;
                            string = "";
                            userInput->setString(string);
                            stage = complete;
                            std::cout << "Done with the input" << std::endl;
                            playerCount = 1;
                            titlebar->setString("Chat room for game: " + network->gameName + "          Players: "
                                                + std::to_string(playerCount) + "             Global IP: "
                                                + hostGlobalIP + " Local IP: " + hostLocalIP);
                            startGameButton->setText("Start");
                            gameNameText->setString("");
                        } else if (!string.empty()) {
                            sf::Packet playerNamePacket;
                            playerNamePacket << string;
                            string = "";
                            userInput->setString(string);
                            stage = complete;
                            network->packetQMutex.lock();
                            network->clientSocket.setBlocking(true);
                            (void) network->clientSocket.send(playerNamePacket);
                            network->packetQMutex.unlock();
                            network->UdpSocket.unbind();
                        }
                    }
                    break;
                }

            case ipInput:
                {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
                        sf::Socket::Status status = sf::Socket::Status::Error;
                        if (auto addrs = sf::Dns::resolve(string); addrs && !addrs->empty()) {
                            std::cout << "Trying to join: " << addrs->front().toString() << std::endl;
                            network->clientSocket.setBlocking(true);
                            status = network->clientSocket.connect(addrs->front(), network->TCPPORT);
                        }

                        if (status == sf::Socket::Status::Done) {
                            std::cout << "TCP Connection established!" << std::endl;
                            gameNameText->setString(
                                "Found a game with name: " + network->gameName + "\nEnter your name");
                            gameNameText->setPosition(
                                {window.getSize().x / 2.0f - (gameNameText->getGlobalBounds().size.x / 2.0f),
                                    window.getSize().y / 10.0f});
                            string = "";
                            userInput->setString(string);
                            stage = nameInput;
                        } else {
                            std::cout << "Failed to join game" << std::endl;
                            string = "";
                            userInput->setString(string);
                            stage = setup;
                        }
                    }
                    break;
                }
            }
        }

        window.clear(sf::Color::Black);
        window.draw(*background);
        if (stage == setup) {
            joinButton->draw(window);
            joinManuallyButton->draw(window);
            hostButton->draw(window);
        } else {
            window.draw(*inputTag);
        }

        window.draw(*gameNameText);
        window.draw(*userInput);
        window.display();
    }
}

void Lobby::process(sf::RenderWindow& window) {
    userInput->setCharacterSize(32);
    userInput->setPosition({120, window.getView().getSize().y - 40});

    inputTag->setCharacterSize(32);
    inputTag->setPosition({0, window.getView().getSize().y - 40});

    std::string string;
    sf::Packet payload;
    sf::Packet updatePacket;
    int packetType;
    while (window.isOpen()) {
        packetType = 0;
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                currentScreenState = screenState::TERMINATE;
                return;
            }

            sf::Vector2f mousePos(sf::Mouse::getPosition(window));
            if (startGameButton->getButtonGlobalBounds().contains(mousePos)) {
                startGameButton->setTextColor(sf::Color::Yellow);
            } else {
                startGameButton->setTextColor(sf::Color::White);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
                string.erase(std::remove(string.begin(), string.end(), '\n'), string.end());
                string.erase(std::remove(string.begin(), string.end(), '\r'), string.end());
                if (!network->HOST && !string.empty()) {
                    payload.clear();
                    payload << 1337 << string;
                    network->sendPacket(payload, false);
                    messageVector.push_back("You: " + string);
                    string = "";
                    userInput->setString("");
                } else if (!string.empty()) {
                    payload.clear();
                    payload << PACKET_MESSAGE << string << "Host";
                    network->sendPacket(payload, false);
                    messageVector.push_back("You: " + string);
                    string = "";
                    userInput->setString("");
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && startIntervall > 0) {
                startIntervall--;
                manualScroll = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
                if ((int) messageVector.size() - startIntervall >= endIntervall) {
                    manualScroll = false;
                } else if ((int) messageVector.size() < startIntervall) {
                    startIntervall++;
                    manualScroll = true;
                }
            }

            if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
                if (textEntered->unicode < 128 && textEntered->unicode != 8) {
                    string += static_cast<char>(textEntered->unicode);
                    string.erase(std::remove(string.begin(), string.end(), '\n'), string.end());
                    userInput->setString(string);
                } else if (textEntered->unicode == 8 && !string.empty()) {
                    string.pop_back();
                    userInput->setString(string);
                }
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)
                && startGameButton->getButtonGlobalBounds().contains(mousePos) && network->HOST) {
                std::cout << "Start game pressed" << std::endl;
                sf::Packet packet;
                network->packetQMutex.lock();
                for (auto& client : network->clients) {
                    network->playerNumber++;
                    packet << PACKET_START << network->playerNumber;
                    client->socket.setBlocking(true);
                    (void) client->socket.send(packet);
                    packet.clear();
                }
                network->packetQMutex.unlock();
                network->playerNumber = 0; // Host always 0
                currentScreenState    = screenState::GAME;
                return;
            }
        }
        if (network->HOST) {
            if (network->listen()) {
                playerCount++;
                titlebar->setString("Chat room for game: " + network->gameName
                                    + "          Players: " + std::to_string(playerCount)
                                    + "             Global IP: " + hostGlobalIP + " Local IP: " + hostLocalIP);
                payload.clear();
                payload << PACKET_UPDATE << playerCount << hostLocalIP << hostGlobalIP;
                network->sendPacket(payload, false);
                std::cout << "Players: " << network->clients.size() << std::endl;
            }
            network->receiveClientMessage(false, messageVector);
        } else {
            payload.clear();
            packetType = 0;
            payload    = network->receivePacket(false);
            payload >> packetType;
            if (packetType != 0) {
                std::cout << "Type: " << packetType << std::endl;
                if (packetType == PACKET_MESSAGE) {
                    std::string fromPlayer, message, fullMessage;
                    payload >> message >> fromPlayer;
                    std::cout << fromPlayer << ": " << message << std::endl;
                    fullMessage = fromPlayer + ": " + message;
                    messageVector.push_back(fullMessage);
                } else if (packetType == PACKET_UPDATE) {
                    payload >> playerCount >> hostLocalIP >> hostGlobalIP;
                    titlebar->setString("Chat room for game: " + network->gameName
                                        + "          Players: " + std::to_string(playerCount)
                                        + "             Global IP: " + hostGlobalIP + "   Local IP: " + hostLocalIP);
                } else if (packetType == PACKET_START) {
                    payload >> network->playerNumber;
                    currentScreenState = screenState::GAME;
                    return;
                }
            }
        }
        window.clear(sf::Color::Black);
        window.draw(*background);
        window.draw(chatBox);
        window.draw(*titlebar);
        window.draw(*inputTag);

        position = 0;
        for (int element = startIntervall;
            (position * 50 + 60) <= (int) window.getView().getSize().y - 50 && element != (int) messageVector.size();
            element++) {
            textElement->setString(messageVector.at(element));
            textElement->setPosition({60, float(position) * 50 + 60});
            window.draw(*textElement);
            position++;
            if ((float(position) * 50 + 60) > window.getView().getSize().y - 50) {
                if (!manualScroll) {
                    startIntervall++;
                }
            }
        }
        window.draw(*userInput);
        if (network->HOST) {
            startGameButton->draw(window);
        }
        window.display();
    }
}
