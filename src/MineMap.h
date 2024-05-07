#ifndef MINEMAP_H
#define MINEMAP_H

#include "Tile.h"
#include <vector>
#include <cstdlib>
#include <ctime>

#define TILE_SIZE 64
#define MAX_BOARD_WIDTH 25
#define MAX_BOARD_HEIGHT 15

class MineMap {
public:
    MineMap(int mapWidth, int mapHeight, int mineAmt);
    std::vector<std::vector<Tile>> tiles;
    short mapWidth;
    short mapHeight;
    short mineAmt;
    short minesFlagged;

    void countNeighbourBombs();

private:
    void initTiles();
    void randomiseTiles();
};

#endif
