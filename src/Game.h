#ifndef GAME_H
#define GAME_H

#ifdef DEBUG
#define LOG(x) std::cout << x << std::endl;
#define ERROR(x) std::cerr << x << std::endl << SDL_GetError() << std::endl;
#else
#define LOG(x)
#define ERROR(x)
#endif

#ifdef DEBUG
#include <iostream>
#endif

#include <chrono>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include "MineMap.h"

#define MAX_NUM_TEXTURE_LENGTH 8
#define MENU_WIDTH 200

class Game {
public:

    Game();
    ~Game();

    int init();
    int initTextures();
    void initImGui();
    void startNewGame(int newWidth, int newHeight, int newBombAmt);
    int run();

    void drawBoard();

    void drawBombs();

private:

    bool firstLeftPress;
    bool gameOver;
    bool gameWon;
    bool invalidInput;
    float mouseX;
    float mouseY;
    int displayTime = 0;

    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();

    MineMap gameMap = MineMap(10, 10, 30);
    SDL_Window *window;
    bool isRunning;
    SDL_Renderer *renderer;
    SDL_Event e;

    SDL_Texture* bombTexture;
    SDL_Texture* flagTexture;
    SDL_Texture* hoverFlagTexture;
    SDL_Texture* closedTexture;
    SDL_Texture* hoverClosedTexture;
    SDL_Texture* openedTexture;

    SDL_Texture* numberTextures[MAX_NUM_TEXTURE_LENGTH];

    SDL_AudioSpec bombSoundSpec;
    uint8_t *bombSoundBuffer;
    uint32_t bombSoundLength;

    SDL_AudioSpec clickSoundSpec;
    uint8_t *clickSoundBuffer;
    uint32_t clickSoundLength;

    void handleMouseClick(int tileX, int tileY, bool isLeftClick);

    void openTilesRecursive(int tileX, int tileY);

    bool checkWin();
};

#endif
