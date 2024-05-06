#ifndef TILE_H
#define TILE_H

#include <SDL_rect.h>

class Tile {
public:
    explicit Tile(bool hasBomb, float x, float y, float size);

    SDL_FRect position;

    bool isFlagged;
    bool hasBomb;
    bool isOpened;
    short bombNeighbours;
};

#endif