#include "MineMap.h"
#include "Game.h"

int main() {
    Game game;
    if (const int status = game.init()) {
        return status;
    }
    return game.run();
}
