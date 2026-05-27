//
// Created by VegarK on 09.11.2017.
//


#include "SquadIO.h"

#include "../Game/Board/Board.h"
#include "../Networking/Network.h"
#include "../StateMachine/Squad.h"
#include "../Support/ActionHandler.h"
#include <fstream>
#include <sstream>

std::vector<unitBase::unitClass> squadIO::readFile(const std::string& fPath) {
    if (!validUnitFile(fPath)) {
        std::cout << "File not valid, using default units" << std::endl;
        std::vector<unitBase::unitClass> units = defaultUnitList();
        writeSquad(units);
        return units;
    }

    int unitId    = 0;
    std::vector<unitBase::unitClass> units;

    std::string unitInfo, currentInfo;
    unitBase::unitClass unitClass;
    std::fstream stream;
    stream.open(fPath);
    while (std::getline(stream, unitInfo)) {
        if (unitInfo.empty()) {
            continue;
        }
        std::stringstream splitStream(unitInfo);

        std::getline(splitStream, currentInfo, ' ');
        unitClass.maxHealth = std::stoi(currentInfo);
        unitClass.health    = unitClass.maxHealth;

        std::getline(splitStream, currentInfo, ' ');
        unitClass.sightRange = std::stoi(currentInfo);

        std::getline(splitStream, currentInfo, ' ');
        unitClass.actionPoints = std::stoi(currentInfo);

        std::getline(splitStream, currentInfo, ' ');
        switch (std::stoi(currentInfo)) {
        case weaponBase::rifleType:
            unitClass.weapon = weaponBase::rifleType;
            break;
        case weaponBase::shotgunType:
            unitClass.weapon = weaponBase::shotgunType;
            break;
        case weaponBase::grenadeType:
            unitClass.weapon = weaponBase::grenadeType;
            break;
        default:
            unitClass.weapon = weaponBase::noType;
            break;
        }

        unitClass.unitId = unitId++;
        units.push_back(unitClass);
    }
    stream.close();
    return units;
}

std::vector<unitBase> squadIO::makeUnits(const std::string& fPath, int playerNumber, ISession* session, Board& gameBoard) {
    unitBase unit;
    std::vector<unitBase> units;
    int id = 0;

    std::vector<unitBase::unitClass> unitList = readFile(fPath);
    for (auto& unitClass : unitList) {
        unitClass.playerId = playerNumber;
        unitClass.health   = unitClass.maxHealth;
        unitClass.unitId   = id++;
        unit               = unitBase(playerNumber, unitClass, gameBoard);
        units.push_back(unit);
        actionHandler::sendUnit(unit, session, unitClass);
    }
    return units;
}

std::vector<unitBase::unitClass> squadIO::defaultUnitList() {
    unitBase::unitClass unit;
    unit.sightRange   = 7;
    unit.actionPoints = 11;
    unit.maxHealth    = 120;

    unitBase::unitClass unit2 = unit;

    unit.weapon  = weaponBase::weaponType::rifleType;
    unit2.weapon = weaponBase::weaponType::shotgunType;

    std::vector<unitBase::unitClass> units;
    units.push_back(unit);
    units.push_back(unit2);

    return units;
}

void squadIO::writeSquad(const std::vector<unitBase::unitClass>& units) {
    std::ofstream stream;
    stream.open("SquadBuilder/squad.txt", std::ofstream::out | std::ofstream::trunc);
    if (!stream.is_open()) {
        std::cout << "writeSquad: failed to open SquadBuilder/squad.txt" << std::endl;
        return;
    }

    for (const auto& unit : units) {
        stream << unit.maxHealth << ' ' << unit.sightRange << ' ' << unit.actionPoints << ' ' << unit.weapon
               << std::endl;
    }
    std::cout << "writeSquad: done" << std::endl;
    stream.close();
}

bool squadIO::validUnitFile(const std::string& fPath) {
    std::cout << "Validating file" << std::endl;

    std::string unitInfo, currentInfo;
    std::fstream stream;
    stream.open(fPath);
    if (!stream.is_open()) {
        std::cout << "Validation: failed to open squad file at path: " << fPath << std::endl;
        return false;
    }

    while (std::getline(stream, unitInfo)) {
        if (unitInfo.empty()) {
            continue;
        }

        std::stringstream splitStream(unitInfo);

        for (int i = 0; i < 4; i++) {
            std::getline(splitStream, currentInfo, ' ');
            if (currentInfo.empty()) {
                return false;
            }
        }
    }
    std::cout << "Valid!" << std::endl;
    return true;
}
