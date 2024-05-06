#include "MineMap.h"

#include <iostream>

MineMap::MineMap(int mapWidth, int mapHeight, int mineAmt) {
    srand(time(nullptr));
    this->mapHeight = mapHeight;
    this->mapWidth = mapWidth;
    this->mineAmt = mineAmt;
    this->initTiles();
    this->randomiseTiles();
    this->countNeighbourBombs();
}

void MineMap::initTiles() {
    for (int i = 0; i < mapHeight; i++) {
        std::vector<Tile> temp;
        for (int j = 0; j < mapWidth; j++) {
            temp.emplace_back(false, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE);
        }
        tiles.emplace_back(temp);
    }
    std::cout << tiles.size() << std::endl;
    std::cout << tiles[1].size() << std::endl;
}

void MineMap::randomiseTiles() {
    short pMineAmt = 0;
    while (pMineAmt < mineAmt) {
        int i = rand() % mapWidth;
        int j = rand() % mapHeight;

        if(!tiles[j][i].hasBomb) {
            tiles[j][i].hasBomb = true;
            pMineAmt++;
        }
    }
}

void MineMap::countNeighbourBombs() {
    for (int i = 0; i < mapWidth; ++i) {
        for (int j = 0; j < mapHeight; ++j) {
            short counter = 0;
            if (i && j) { // lefttop (0,0)
                if (tiles[j-1][i-1].hasBomb) {
                    counter++;
                }
            }
            if (i && j != mapHeight - 1) { // righttop
                if (tiles[j+1][i-1].hasBomb) {
                    counter++;
                }
            }
            if (i != mapWidth - 1 && j != mapHeight - 1) { // rightbottom
                if (tiles[j+1][i+1].hasBomb) {
                    counter++;
                }
            }
            if (i != mapWidth - 1 && j) { // leftbottom
                if(tiles[j-1][i+1].hasBomb) {
                    counter++;
                }
            }
            if (i) { // topbound only
                if(tiles[j][i-1].hasBomb) {
                    counter++;
                }
            }
            if (j != mapHeight - 1) { // rightbound only
                if(tiles[j+1][i].hasBomb) {
                    counter++;
                }
            }
            if (i != mapWidth - 1) { // bottombound
                if(tiles[j][i+1].hasBomb) {
                    counter++;
                }
            }
            if (j) {
                if(tiles[j-1][i].hasBomb) {
                    counter++;
                }
            }
            tiles[j][i].bombNeighbours = counter;
        }
    }
}