//
// Created by VegarK on 18.10.2017.
//

#ifndef DAT220_PROJECT_ACTIONHANDLER_H
#define DAT220_PROJECT_ACTIONHANDLER_H


#include <SFML/Network/Packet.hpp>
#include <iostream>
#include <cmath>
#include <Game/Vision/Vision.h>
#include "MathHelper.h"
#include "Game/Objects/Units/UnitBase.h"

class actionHandler {
public:
    static std::vector<sf::Packet> actionLog;
    /**
     * Finds a unit based on id and playernumber
     * @param id the unit's id
     * @param player the player who owns the unit
     * @param unitList The actual list of units to pick from
     * @return A pointer to the unit specified by id and player
     */
    static unitBase *getUnit(int id, int player, std::vector<unitBase> *unitList);

    /**
     *Move's a unit from its current position to the specified one
     * @param unit The unit to be moved
     * @param xPos The x coordinate of the tile to be moved to
     * @param yPos The y coordinate of the tile to be moved to
     * @param tileMap The map of tiles in which the unit moves
     * @return Returns a packet which is used to perform the same action at remote clients
     */
    static std::vector<sf::Packet> Move(unitBase &unit, float xPos, float yPos, std::vector<std::vector<Tile>> &tileMap);

    /**
     * Fire a unit's weapon
     * @param unit The unit which will fire
     * @param target A set of coordinates showing where the shot will be fired
     * @param units A list of units, to check for a hit
     * @param board The gameBoard, which is necessary to check for collisions
     * @return Returns a packet which is used to perform the same action at remote clients
     */
    static std::vector<sf::Packet> Shoot(unitBase &unit, sf::Vector2f target, std::vector<unitBase> &units, Board &board);

    /**
     * Swaps a unit's weapon
     * @param unit The unit which will swap weapon
     * @param type The weapon the unit will swap to
     * @return Returns a packet which is used to perform the same action at remote clients
     */
    static std::vector<sf::Packet> weaponSwap(unitBase *unit, weaponBase::weaponType type);

    /**
     * Reads received packages all of which process the opponent's moves.
     * @param packet The packet, containing one identifier for functions and the necessary parameters for the function
     * @param unitList A list of units, to be passed on to certain functions
     * @param network A pointer to network, to be used when receiving units to be produced
     * @param gameBoard A reference to board, to be used when receiving units to be produced
     * @param turn A bool used to check if it's your turn
     * @return
     */
    static void GetRemoteAction(sf::Packet packet, std::vector<unitBase> *unitList, Network *network, Board &gameBoard);

    /**
     * Sends the information to create a unit to a remote client
     * @param unit The unit to be transferred
     * @param network A pointer to network, so the package can be sent
     * @param unitClass Contains the "class" variables for the unit, containing base stats
     */
    static void sendUnit(unitBase unit, Network *network, unitBase::unitClass unitClass);

    /**
     * Check for dead units and see if win condition is met
     * @param unitList Reference to the unit list
     * @param network Reference to the network object
     * @return Returns nothing
     */
    static void victory(std::vector<unitBase> &unitList, Network *network);

    /**
     * Passes the turn to the next player
     * @param player Id of player who just ended is turn
     * @param network Reference to network object
     * @param unitList Reference to the unit list
     * @return Returns nothing
     */
    static void nextTurn(int player, Network &network, std::vector<unitBase> *unitList);
protected:
    /**
     * Checks collision between bullets and unit
     * @param unit Units hitbox
     * @param unit bullets hitbox
     * @return Returns true if collision, false if not
     */
    static bool ShootingCollision(sf::CircleShape unit, sf::RectangleShape bullet);

};


#endif //DAT220_PROJECT_ACTIONHANDLER_H
