#include "Tile.h"

Tile::Tile(const bool hasBomb, const float x, const float y, const float size) {
    this->hasBomb = hasBomb;
    this->isFlagged = false;
    this->position.x = x;
    this->position.y = y;
    this->position.w = size;
    this->position.h = size;

    isOpened = false;
    bombNeighbours = 0;
}
