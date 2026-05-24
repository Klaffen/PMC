# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

PMC ("Project Merge Conflict") is a turn-based tactical shooter built in C++17 with SFML. Two players connect over a network and take turns moving units and firing weapons on a tile-based board.

## Build

SFML 2.6.2 is fetched automatically via CMake's `FetchContent` — no separate SFML installation needed.

```sh
# Configure (first time or after CMakeLists changes)
cmake -B cmake-build-debug -S .

# Build
cmake --build cmake-build-debug

# Run
.\cmake-build-debug\DAT220_project.exe
```

The project was developed in CLion; the `cmake-build-debug/` directory is the standard out-of-source build folder. There are no tests.

## Architecture

### State machine (`StateMachine/`)

`main.cpp` owns a `std::vector<StatePtr>` indexed by `State::screenState` enum (`MENU=0, GAME=1, LOBBY=2, SETTINGS=3, SQUAD=4, TERMINATE=5`). Each state implements `enter(window)` (setup + inner loop) and `process(window)` (one frame update). `enter()` returns the next `screenState` integer to drive transitions.

### Gameplay (`Game/`)

- **`Board/`** — `Board` holds a 2D `tileMap` of `Tile` objects and a `Level` (obstacle layout). `Shape3D` represents isometric-style obstacles drawn on tiles.
- **`Objects/Units/UnitBase`** — the single unit class; carries a `std::vector<shared_ptr<weaponBase>>` (Rifle, Grenade, Shotgun) and delegates movement to `Pathfinding` and sight to `Vision`.
- **`Objects/Weapons/`** — `weaponBase` is the base; concrete weapons differ in range, damage, and projectile behavior.
- **`Pathfinding/`** — Dijkstra-based. `getMovementMap()` returns a flat `map<tileIndex, cost>` for reachable tiles; `getPath()` reconstructs a tile sequence.
- **`Vision/`** — line-of-sight raycasting; results stored per-unit in `visibleTiles`.
- **`UserInterface`** / **`Input`** — HUD rendering and keyboard/mouse event translation for in-game actions.
- **`GameSettings`** — static helpers for resolution, fullscreen toggle, and SFML audio playback.

### Networking (`Networking/`)

`Network` wraps an `sf::UdpSocket` (gameplay packets, port 50000) and an `sf::TcpListener`/`sf::TcpSocket` pair (lobby, port 51000). A background `sf::Thread` drains the `packetQ` send queue. `Client` represents a connected peer.

### Action serialization (`Support/ActionHandler`)

`actionHandler` is the bridge between gameplay and networking. Every local action (Move, Shoot, weaponSwap, nextTurn) produces `std::vector<sf::Packet>` that are both applied locally and queued for transmission. `GetRemoteAction()` deserializes incoming packets and replays them.

### Squad persistence (`SquadBuilder/`)

`squadIO` reads/writes `SquadBuilder/squad.txt`, a plain-text file storing `unitClass` configs (weapon type, stats, cost). The Squad state (`StateMachine/Squad`) lets the player build their squad before a match; `squadIO::defaultUnitList()` is used as fallback when the file is absent or invalid.

## Asset paths

All runtime assets are under `Data/` (relative to the executable's working directory):
- `Data/Images/` — backgrounds, explosion sprite, keybindings overlay
- `Data/Fonts/` — three `.ttf`/`.otf` fonts
- `Data/Audio/` — weapon sounds and background music