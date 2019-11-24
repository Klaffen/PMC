#include "Pathfinding.h"

#define INF 9999


std::map<int, int> Pathfinding::getMovementMap(Tile &start, int maxMovement,
                                               std::vector<std::vector<Tile>> &tileMap) {

    std::map<int, std::map<int, int>> vertices = getTileGraph(start, maxMovement, tileMap);

    std::map<int, int> distance;
    std::map<int, int> previous;
    std::vector<int> nodes;

    for (auto &vertex : vertices) {
        if (vertex.first == start.id) {
            distance[vertex.first] = 0; //Distance from start to start
            previous[vertex.first] = -1; //Start tile has no previous;
        } else {
            distance[vertex.first] = INF; //Unknown distance from start to vertex
            previous[vertex.first] = INF; //Unknown previous to vertex
        }
        nodes.push_back(vertex.first);
    }

    //bool pathFound = false;
    while (!nodes.empty()) {
        //Find node with lowest distance value
        int currentMinNode = findShortestDistance(nodes, distance);
        //Remove it from nodes so we don't visit the same node twice
        auto position = std::find(nodes.begin(), nodes.end(), currentMinNode);
        if (position != nodes.end())
            nodes.erase(position);

        //iterate through neighbour nodes and calculate new cost (current node cost + neighbour cost)
        for (auto &neighbour : vertices[currentMinNode]) {
            int alternativeDistance = distance[currentMinNode] + neighbour.second;
            if (alternativeDistance < distance[neighbour.first]) {
                distance[neighbour.first] = alternativeDistance;
                previous[neighbour.first] = currentMinNode;
            }
        }
    }

    return previous;
}

std::vector<sf::Vector2i> Pathfinding::getPath(Tile &start, Tile &goal,
                                               std::map<int, int> movementMap,
                                               int maxMovement,
                                               std::vector<std::vector<Tile>> &tileMap) {

    if (goal.isTaken || goal.hasUnit || start == goal || movementMap.empty()) return {};

    std::vector<int> path = std::vector<int>();

    //Check if goal exist in movementMap
    bool goalExist = false;
    for (auto &tile : movementMap) {
        if (tile.first == goal.id) {
            goalExist = true;
        }
    }

    std::vector<sf::Vector2i> tilePath = std::vector<sf::Vector2i>();
    if (goalExist) {
        //Construct a path
        path.push_back(goal.id);
        int nextNode = movementMap[goal.id];
        while (nextNode != start.id && nextNode != -1) {
            path.push_back(nextNode);
            nextNode = movementMap[nextNode];
        }
        path.push_back(start.id);

        if ((int) path.size() > maxMovement + 1) return {};

        //When you construct the path you are going from goal to start, reversing gives start->goal
        std::reverse(path.begin(), path.end());

        tilePath = getTileCords(path, tileMap);
    }

    return tilePath;
}

std::map<int, int> Pathfinding::neighbourCost(Tile &tile, std::vector<std::vector<Tile>> &tileMap) {
    std::map<int, int> neighbours;
    sf::Vector2i tilePos = getTileIndex(tile);

    int x = tilePos.x;
    int y = tilePos.y;

    int xSize = (int) tileMap.size() - 1;
    int ySize = (int) tileMap.at(0).size() - 1;

    if (x > 0) {
        x = tilePos.x - 1;
        if (!tileMap[x][y].isTaken && !tileMap[x][y].hasUnit) {
            neighbours.emplace(tileMap[x][y].id, 1);
        } else neighbours.emplace(tileMap[x][y].id, INF);
    }

    x = tilePos.x;

    if (x < xSize) {
        x = tilePos.x + 1;
        if (!tileMap[x][y].isTaken && !tileMap[x][y].hasUnit) {
            neighbours.emplace(tileMap[x][y].id, 1);
        } else neighbours.emplace(tileMap[x][y].id, INF);
    }

    x = tilePos.x; //reset to the first x pos
    if (y > 0) {
        y = tilePos.y - 1;
        if (!tileMap[x][y].isTaken && !tileMap[x][y].hasUnit) {
            neighbours.emplace(tileMap[x][y].id, 1);
        } else neighbours.emplace(tileMap[x][y].id, INF);
    }

    if (y < ySize) {
        y = tilePos.y + 1;
        if (!tileMap[x][y].isTaken && !tileMap[x][y].hasUnit) {
            neighbours.emplace(tileMap[x][y].id, 1);
        } else neighbours.emplace(tileMap[x][y].id, INF);
    }

    return neighbours;
}

std::map<int, std::map<int, int>> Pathfinding::getTileGraph(Tile &start, int maxMovement, std::vector<std::vector<Tile>> &tileMap) {
    sf::Vector2i startIndex = getTileIndex(start);
    std::map<int, std::map<int, int>> tileGraph; // (example) Tile A, has connections {{B, 1}, {C, 1}, {D, INF}, {E, 1}} where numbers are cost to move there
    for (auto tileList : tileMap) {
        for (Tile &tile: tileList) {
            std::map<int, int> cost;
            sf::Vector2i tileIndex = getTileIndex(tile);
            if (tileIndex.x <= startIndex.x + maxMovement && tileIndex.x >= startIndex.x - maxMovement) {
                if (tileIndex.y <= startIndex.y + maxMovement && tileIndex.y >= startIndex.y - maxMovement) {
                    cost = neighbourCost(tile, tileMap);
                    tileGraph.emplace(tile.id, cost);
                }
            }
        }
    }
    return tileGraph;
}

sf::Vector2i Pathfinding::getTileIndex(Tile &position) {
    sf::Vector2i index((int) (position.getPosition().x / Board::TILE_SIZE),
                       (int) (position.getPosition().y / Board::TILE_SIZE));
    return index;
}

int Pathfinding::findShortestDistance(std::vector<int> &nodes, std::map<int, int> distance) {

    int currentMin = std::numeric_limits<int>::max();
    int minNode = std::numeric_limits<int>::max();
    for (auto &node : nodes) {
        if (distance[node] < currentMin) {
            currentMin = distance[node];
            minNode = node;
        }
    }

    return minNode;
}

std::vector<sf::Vector2i> Pathfinding::getTileCords(std::vector<int> &path, std::vector<std::vector<Tile>> &tileMap) {

    std::vector<sf::Vector2i> tilepath;

    for (auto &p : path) {
        for (auto &tile : tileMap) {
            for (auto &&item : tile) {
                if (item.id == p) {
                    sf::Vector2i tileIndex = getTileIndex(item);
                    tilepath.push_back(tileIndex);
                }
            }
        }
    }

    return tilepath;
}