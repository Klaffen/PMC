cmake_minimum_required(VERSION 3.8)
project(DAT220_project)

set(CMAKE_CXX_STANDARD 11)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

set(SOURCE_FILES main.cpp StateMachine/Game.h StateMachine/Menu.h StateMachine/Settings.h
        StateMachine/State.h StateMachine/Menu.cpp
        StateMachine/Game.cpp StateMachine/Settings.cpp Game/Objects/Units/UnitBase.cpp
        Game/Objects/Units/UnitBase.h Game/Objects/Weapons/WeaponBase.cpp Game/Objects/Weapons/WeaponBase.h
        Networking/Network.cpp Networking/Network.h Networking/Client.h
        StateMachine/Lobby.cpp StateMachine/Lobby.h
        Game/Board/Board.h Game/Board/Tile.h
        Game/Board/Shape3D.h Game/Board/Board.cpp Game/Board/Shape3D.cpp
        Game/Board/Tile.cpp Support/ActionHandler.cpp Support/ActionHandler.h
        Game/Objects/Weapons/Rifle.cpp Game/Objects/Weapons/Rifle.h Support/Button.cpp
        Support/Button.h
        Game/Input.cpp Game/Input.h Game/Objects/Weapons/Grenade.cpp
        Game/Objects/Weapons/Grenade.h
        Game/Vision/Vision.cpp Game/Vision/Vision.h Game/Board/Levels/Level.cpp
        Game/UserInterface.cpp Game/UserInterface.h
        Game/Pathfinding/Pathfinding.cpp Game/Pathfinding/Pathfinding.h
        Game/Objects/Weapons/Shotgun.cpp Game/Objects/Weapons/Shotgun.h
        SquadBuilder/SquadIO.cpp SquadBuilder/SquadIO.h
        Support/MathHelper.cpp Support/MathHelper.h
        StateMachine/Squad.cpp StateMachine/Squad.h
        Game/GameSettings.cpp Game/GameSettings.h)

add_executable(DAT220_project ${SOURCE_FILES} Game/Board/Levels/Level.cpp Game/Board/Levels/Level.h)

target_link_libraries(DAT220_project sfml-graphics sfml-window sfml-system sfml-network sfml-audio)

target_include_directories(DAT220_project PUBLIC . plog/include)
