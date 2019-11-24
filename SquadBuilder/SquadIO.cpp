//
// Created by VegarK on 09.11.2017.
//


#include <StateMachine/Squad.h>
#include "SquadIO.h"

std::vector<unitBase::unitClass> squadIO::readFile(const std::string& fPath) {
    if (!validUnitFile(fPath)) {
        std::vector<unitBase::unitClass> units = defaultUnitList();
        writeSquad(units);
        return units;
    }
    int buildCost = 0;
    int unitId = 0;
    std::vector<unitBase::unitClass> units;

    std::string unitInfo, currentInfo;
    unitBase::unitClass unitClass;
    std::fstream stream;
    stream.open(fPath);
    while (!stream.eof()) {
        std::getline(stream, unitInfo);
        std::stringstream splitStream(unitInfo);

        std::getline(splitStream, currentInfo, ' ');
        unitClass.maxHealth = std::stoi(currentInfo);
        unitClass.health = unitClass.maxHealth;
        buildCost = unitClass.maxHealth / PRICE_HEALTH;

        std::getline(splitStream, currentInfo, ' ');
        unitClass.sightRange = std::stoi(currentInfo);
        buildCost += unitClass.sightRange / PRICE_VISION;

        std::getline(splitStream, currentInfo, ' ');
        unitClass.actionPoints = std::stoi(currentInfo);
        buildCost += unitClass.actionPoints / PRICE_ACTIONPOINT;

        std::getline(splitStream, currentInfo, ' ');
        switch (std::stoi(currentInfo)) {
            case (int)weaponBase::weaponType::rifleType:
                unitClass.weapon = weaponBase::weaponType::rifleType;
                break;
            case (int)weaponBase::weaponType::shotgunType:
                unitClass.weapon = weaponBase::weaponType::shotgunType;
                break;
            case (int)weaponBase::weaponType::grenadeType:
                unitClass.weapon = weaponBase::weaponType::grenadeType;
                break;
            default:unitClass.weapon = weaponBase::weaponType::noType;
                break;
        }

        if (buildCost > MAX_BUILD_POINTS) {
            return defaultUnitList();
        }
        unitClass.unitId = unitId++;
        units.push_back(unitClass);
    }
    stream.close();
    return units;
}

std::vector<unitBase> squadIO::makeUnits(const std::string& fPath, Network *net, Board &gameBoard) {
    unitBase unit;
    std::vector<unitBase> units;
    int id = 0;

    std::vector<unitBase::unitClass> unitList = readFile(fPath);
    for (auto &unitClass : unitList) {
        unitClass.playerId = net->playerNumber;
        unitClass.health = unitClass.maxHealth;
        unit = unitBase(net, unitClass, gameBoard);
        units.push_back(unit);
        actionHandler::sendUnit(unit, net, unitClass);
        id++;
    }
    return units;
}

std::vector<unitBase::unitClass> squadIO::defaultUnitList() {
    unitBase::unitClass unit;
    unit.actionPoints = 11;
    unit.sightRange = 7;
    unit.actionPoints = 11;
    unit.maxHealth = 120;

    unitBase::unitClass unit2 = unit;

    unit.weapon = weaponBase::weaponType::rifleType;
    unit2.weapon = weaponBase::weaponType::shotgunType;

    std::vector<unitBase::unitClass> units;
    units.push_back(unit);
    units.push_back(unit2);

    return units;
}

void squadIO::writeSquad(std::vector<unitBase::unitClass> units) {
    std::ofstream stream;
    stream.open("SquadBuilder/squad.txt", std::ofstream::out | std::ofstream::trunc);
    for (unsigned long i = 0; i < units.size(); i++) {
        auto unit = units.at(i);
        stream << unit.health << ' ' << unit.sightRange << ' ' << unit.actionPoints;
        if (unit.weapon == weaponBase::noType)
            stream << 0;
        else if (unit.weapon == weaponBase::rifleType)
            stream << 1;
        else if (unit.weapon == weaponBase::shotgunType)
            stream << 2;
        else if (unit.weapon == weaponBase::grenadeType)
            stream << 3;

        if (i != units.size() - 1)
            stream << std::endl;
    }
    stream.close();
}

bool squadIO::validUnitFile(const std::string &fPath) {
    std::string unitInfo, currentInfo;
    std::fstream stream;
    stream.open(fPath);
    std::cout << "Validating file" << std::endl;
    while (!stream.eof()) {
        std::getline(stream, unitInfo);

        if (unitInfo.empty()) {
            return false;
        }

        std::stringstream splitStream(unitInfo);

        for (int i = 0; i < 4; i++) {
            std::getline(splitStream, currentInfo, ' ');
            if (currentInfo.empty()) {
                return false;
            }
        }
        std::getline(splitStream, currentInfo, ' ');
        if (!currentInfo.empty())
            return false;
    }
    std::cout << "Valid!" << std::endl;
    return true;
}