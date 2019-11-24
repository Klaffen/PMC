//
// Created by VegarK on 09.11.2017.
//

#ifndef DAT220_PROJECT_SQUADREADER_H
#define DAT220_PROJECT_SQUADREADER_H

#include <sstream>
#include <fstream>
#include "Support/ActionHandler.h"
#include "Game/Objects/Units/UnitBase.h"

class squadIO {
public:
    static std::vector<unitBase::unitClass> readFile(std::string fPath);
    static std::vector<unitBase> makeUnits(std::string fPath, Network* net, Board &gameBoard);
    static void writeSquad(std::vector<unitBase::unitClass> units);
};


#endif //DAT220_PROJECT_SQUADREADER_H
