//
// Created by VegarK on 09.11.2017.
//


#include "SquadIO.h"

std::vector<unitBase::unitClass> squadIO::readFile(std::string fPath) {
    std::string unitInfo, currentInfo;
    std::vector<weaponBase::weaponType> weapons;
    unitBase::unitClass unitClass;
    std::vector<unitBase::unitClass> units;
    std::fstream stream;
    stream.open(fPath);
    while (!stream.eof()) {
        std::getline(stream, unitInfo);
        std::stringstream splitStream(unitInfo);
        std::getline(splitStream, currentInfo, ' ');
        unitClass.health = std::stoi(currentInfo);
        unitClass.maxHealth = std::stoi(currentInfo);
        std::getline(splitStream, currentInfo, ' ');
        unitClass.sightRange = std::stoi(currentInfo);
        std::getline(splitStream, currentInfo, ' ');
        unitClass.actionPoints = std::stoi(currentInfo);
        std::getline(splitStream, currentInfo, ' ');
        unitClass.builderCost = std::stoi(currentInfo);
        std::getline(splitStream, currentInfo, ' ');
        unitClass.unitId = std::stoi(currentInfo);

        while (!splitStream.eof()) {
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
        }
        units.push_back(unitClass);
        weapons.clear();
    }
    stream.close();
    return units;
}


std::vector<unitBase> squadIO::makeUnits(std::string fPath, Network *net, Board &gameBoard) {
    unitBase unit;
    std::vector<unitBase> units;
    int id = 0;

    std::vector<unitBase::unitClass> unitList = readFile(fPath);
    for (auto &unitClass : unitList) {
        unitClass.playerId = net->playerNumber;
        unit = unitBase(net, unitClass, gameBoard);
        units.push_back(unit);
        actionHandler::sendUnit(unit, net, unitClass);
        id++;
    }
    return units;
}

void squadIO::writeSquad(std::vector<unitBase::unitClass> units) {
    std::ofstream stream;
    stream.open("SquadBuilder/squad.txt", std::ofstream::out | std::ofstream::trunc);
    for (unsigned long i = 0; i < units.size(); i++) {
        auto unit = units.at(i);
        stream << unit.health << ' ' << unit.sightRange << ' ' << unit.actionPoints << ' ' << unit.builderCost << ' ' << i << ' ';
        if (unit.weapon == weaponBase::noType)
            stream << 0;
        else if (unit.weapon == weaponBase::rifleType)
            stream << 1;
        else if (unit.weapon == weaponBase::shotgunType)
            stream << 2;
        else if (unit.weapon == weaponBase::grenadeType)
            stream << 3;

        if (i != units.size()-1)
            stream << std::endl;

    }
    stream.close();
}


