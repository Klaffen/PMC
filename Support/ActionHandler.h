//
// Created by VegarK on 18.10.2017.
//

#ifndef DAT220_PROJECT_ACTIONHANDLER_H
#define DAT220_PROJECT_ACTIONHANDLER_H


#include "../Game/MatchState.h"
#include "../Game/Objects/Units/UnitBase.h"
#include "PacketTypes.h"

#include <SFML/Network/Packet.hpp>

class Network;
class ISession;

class actionHandler {
public:
    static std::vector<sf::Packet> actionLog;
    /**
     * Finds a unit based on id and playernumber
     */
    static unitBase* getUnit(int id, int player, std::vector<unitBase>* unitList);

    /**
     * Moves a unit from its current position to the specified one
     */
    static std::vector<sf::Packet> Move(
        unitBase& unit, float xPos, float yPos, std::vector<std::vector<Tile>>& tileMap);

    /**
     * Fires a unit's weapon
     */
    static std::vector<sf::Packet> Shoot(unitBase& unit, sf::Vector2f target, std::vector<unitBase>& units,
        Board& board, weaponBase::weaponType type);

    /**
     * Swaps a unit's weapon
     */
    static std::vector<sf::Packet> weaponSwap(unitBase* unit, weaponBase::weaponType type);

    /**
     * Deserializes and replays an incoming packet
     */
    static void GetRemoteAction(sf::Packet packet, std::vector<unitBase>* unitList, Network* network,
        Board& gameBoard, MatchState& matchState);

    /**
     * Sends the information to create a unit to a remote client
     */
    static void sendUnit(unitBase unit, ISession* session, unitBase::unitClass unitClass);

    /**
     * Check for dead units and see if win condition is met
     */
    static void victory(std::vector<unitBase>& unitList, Network* network, MatchState& matchState);

    /**
     * Passes the turn to the next player
     */
    static void nextTurn(int player, Network& network, std::vector<unitBase>* unitList, MatchState& matchState);

protected:
    /**
     * Checks collision between a bullet and a unit
     */
    static bool ShootingCollision(const sf::CircleShape& unit, const sf::RectangleShape& bullet);
};


#endif // DAT220_PROJECT_ACTIONHANDLER_H
