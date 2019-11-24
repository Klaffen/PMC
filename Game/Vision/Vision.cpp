//
// Created by Roman Aleksandrov on 30/10/2017.
//

#include "Vision.h"

Vision::Vision() {}

std::vector<Tile> Vision::getVisibleTiles(Tile tile, int visionRadius, Board &board) {

    std::vector<Tile> visibleTiles;
    visibleTiles.clear();

    for (int i = 0; i <= 360; ++i) {
        float angleRad = i * M_PI / 180;

        for (int radius = 0; radius < visionRadius; ++radius) {

            float posX = tile.getPosition().x + radius * Board::TILE_SIZE * cos(angleRad);
            float posY = tile.getPosition().y + radius * Board::TILE_SIZE * sin(angleRad);

            sf::Vector2i targetTileCoord = board.getTileCoord({posX, posY});

            if (targetTileCoord.x < 0 || targetTileCoord.y < 0 || targetTileCoord.x > 59 || targetTileCoord.y > 33)
                break;

            auto it = std::find(visibleTiles.begin(), visibleTiles.end(),
                                board.tileMap[targetTileCoord.x][targetTileCoord.y]);
            if (!board.tileMap[targetTileCoord.x][targetTileCoord.y].isTaken) {
                if (it == visibleTiles.end()) {
                    visibleTiles.push_back(board.tileMap[targetTileCoord.x][targetTileCoord.y]);
                }
            } else {
                visibleTiles.push_back(board.tileMap[targetTileCoord.x][targetTileCoord.y]);
                break;
            };
        }
    }
    return visibleTiles;
}

bool Vision::tileIsInSight(Tile tile, std::vector<Tile> visibleTiles) {
    return std::find(visibleTiles.begin(), visibleTiles.end(), tile) != visibleTiles.end();
}
