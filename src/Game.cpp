#include "Game.h"

Game::Game() {
    window = nullptr;
    renderer = nullptr;
    firstLeftPress = true;
    isRunning = true;
    gameOver = false;
}

Game::~Game() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(bombTexture);
    SDL_DestroyTexture(closedTexture);
    SDL_DestroyTexture(flagTexture);
    SDL_DestroyTexture(hoverClosedTexture);
    SDL_DestroyTexture(hoverFlagTexture);
    SDL_DestroyTexture(openedTexture);
    for (int i = 0; i < MAX_NUM_TEXTURE_LENGTH; ++i) {
        SDL_DestroyTexture(numberTextures[i]);
    }

    SDL_Quit();
}

int Game::init() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        ERROR("SDL Failed to Initialise")
        return 1;
    }
    LOG("Initialised SDL Succesfully")

    window = SDL_CreateWindow("MineSweeper",  gameMap.mapWidth * TILE_SIZE + MENU_WIDTH, gameMap.mapHeight * TILE_SIZE, 0);
    if(window == nullptr) {
        ERROR("Window Failed to Create")
        return 2;
    }
    LOG("Window Created Succesfully")

    renderer = SDL_CreateRenderer(window, nullptr, 0);
    if(renderer == nullptr) {
        ERROR("Renderer Failed to Create")
        return 3;
    }
    LOG("Renderer Created Succesfully")

    if(initTextures()) {
        ERROR("Failed to initialise textures")
        return 4;
    }
    LOG("Initialised Textures Succesfully")

    if(SDL_LoadWAV("res/sfx/explosion.wav", &bombSoundSpec, &bombSoundBuffer, &bombSoundLength)) {
        ERROR("Failed to initialise bombSoundAudio")
    }
    LOG("Initialised bombSoundAudio Succesfully")

    if(SDL_LoadWAV("res/sfx/click.wav", &clickSoundSpec, &clickSoundBuffer, &clickSoundLength)) {
        ERROR("Failed to initialise clickSoundAudio")
    }
    LOG("Initialised clickSoundAudio Succesfully")

    SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_OUTPUT, &bombSoundSpec);

    initImGui();

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    return 0;
}

int Game::initTextures() {
    bombTexture = IMG_LoadTexture(renderer, "res/img/Bomb.png");
    if(bombTexture == nullptr) {
        ERROR("Bombtexture Failed to Create")
        return 1;
    }
    LOG("Bombtexture Created Succesfully");
    if (SDL_SetTextureScaleMode(bombTexture, SDL_SCALEMODE_NEAREST)) {
        ERROR("TextureScalemode Failed to Set")
        return 1;
    }
    LOG("TextureScaleMode Set Succesfully")


    flagTexture = IMG_LoadTexture(renderer, "res/img/Flag.png");
    if(flagTexture == nullptr) {
        ERROR("Flagtexture failed to create")
        return 1;
    }
    LOG("Flagtexture Created Succesfully");
    if (SDL_SetTextureScaleMode(flagTexture, SDL_SCALEMODE_NEAREST)) {
        ERROR("TextureScalemode Failed to Set")
        return 1;
    }
    LOG("TextureScaleMode Set Succesfully")


    hoverFlagTexture = IMG_LoadTexture(renderer, "res/img/HoverFlag.png");
    if(hoverFlagTexture == nullptr) {
        ERROR("HoverFlagTexture failed to create")
        return 1;
    }
    LOG("HoverFlagTexture Created Succesfully")
    if (SDL_SetTextureScaleMode(hoverFlagTexture, SDL_SCALEMODE_NEAREST)) {
        ERROR("TextureScalemode Failed to Set")
        return 1;
    }
    LOG("TextureScaleMode Set Succesfully")


    closedTexture = IMG_LoadTexture(renderer, "res/img/Closed.png");
    if(closedTexture == nullptr) {
        ERROR("closedTexture failed to create")
        return 1;
    }
    LOG("closedTexture Created Succesfully")
    if (SDL_SetTextureScaleMode(closedTexture, SDL_SCALEMODE_NEAREST)) {
        ERROR("TextureScalemode Failed to Set")
        return 1;
    }
    LOG("TextureScaleMode Set Succesfully")


    hoverClosedTexture = IMG_LoadTexture(renderer, "res/img/HoverClosed.png");
    if(hoverClosedTexture == nullptr) {
        ERROR("HoverClosedtexture failed to create")
        return 1;
    }
    LOG("HoverClosedtexture Created Succesfully")
    if (SDL_SetTextureScaleMode(hoverClosedTexture, SDL_SCALEMODE_NEAREST)) {
        ERROR("TextureScalemode Failed to Set")
        return 1;
    }
    LOG("TextureScaleMode Set Succesfully")


    openedTexture = IMG_LoadTexture(renderer, "res/img/Opened.png");
    if(openedTexture == nullptr) {
        ERROR("openedTexture failed to create")
        return 1;
    }
    LOG("openedTexture Created Succesfully")
    if (SDL_SetTextureScaleMode(openedTexture, SDL_SCALEMODE_NEAREST)) {
        ERROR("TextureScalemode Failed to Set")
        return 1;
    }
    LOG("TextureScaleMode Set Succesfully")

    for (int i = 0; i < MAX_NUM_TEXTURE_LENGTH; i++) {
        numberTextures[i] = IMG_LoadTexture(renderer, ("res/img/Numbers/" + std::to_string(i + 1) + ".png").c_str());
        if(numberTextures[i] == nullptr) {
            ERROR("numberTextures[" + std::to_string(i) + "] failed to create")
            return 1;
        }
        LOG("numberTextures[" + std::to_string(i) + "] Created Succesfully")
        if (SDL_SetTextureScaleMode(numberTextures[i], SDL_SCALEMODE_NEAREST)) {
            ERROR("TextureScalemode Failed to Set")
            return 1;
        }
        LOG("TextureScaleMode Set Succesfully")
    }

    return 0;
}

void Game::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Set ImGui theme
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
}

void Game::startNewGame(int newWidth, int newHeight, int newBombAmt) {
    if (newWidth < 5 || newWidth > 20) {
        LOG("Invalid width")
        invalidInput = true;
        return;
    }
    if (newHeight < 5 || newHeight > 20) {
        LOG("Invalid height")
        invalidInput = true;
        return;
    }
    if (newBombAmt < 5 || newBombAmt > newWidth * newHeight) {
        LOG("Invalid BombAmt")
        invalidInput = true;
        return;
    }
    invalidInput = false;

    startTime = std::chrono::system_clock::now();
    gameWon = false;
    gameOver = false;
    firstLeftPress = true;
    gameMap = MineMap(newWidth, newHeight, newBombAmt);
    SDL_SetWindowSize(window, newWidth * TILE_SIZE + MENU_WIDTH, newHeight * TILE_SIZE);
}

int Game::run() {
    std::chrono::duration<double> elapsedTime;

    while (isRunning) {
        if(!gameWon && !gameOver) {
            elapsedTime = std::chrono::system_clock::now() - startTime;
        }
        SDL_GetMouseState(&mouseX, &mouseY);
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL3_ProcessEvent(&e);
            switch (e.type) {
                case SDL_EVENT_QUIT:
                    isRunning = false;
                    break;
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    if(e.window.windowID == SDL_GetWindowID(window)) {
                        isRunning = false;
                    }
                    break;
                case SDL_EVENT_KEY_DOWN:
                    switch (e.key.keysym.sym) {
                        #ifdef DEBUG
                        case SDLK_x:
                            gameOver = !gameOver;
                            break;
                        #endif
                        default:
                            break;
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    switch (e.button.button) {
                        case SDL_BUTTON_LEFT:
                            if(!gameOver && !gameWon) {
                                handleMouseClick((int)mouseX / TILE_SIZE, (int)mouseY / TILE_SIZE, true);
                                if(checkWin()) {
                                    LOG("Game Won")
                                    gameWon = true;
                                }
                            }

                            break;
                        case SDL_BUTTON_RIGHT:
                            if (!gameOver && !gameWon) {
                                handleMouseClick((int)mouseX / TILE_SIZE, (int)mouseY / TILE_SIZE, false);
                            }
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Create ImGui Windows
        ImGui::SetNextWindowSize(ImVec2(MENU_WIDTH, gameMap.mapHeight * TILE_SIZE));
        ImGui::SetNextWindowPos(ImVec2(gameMap.mapWidth * TILE_SIZE, 0));
        ImGui::Begin("New Game", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        // ImGui::Checkbox("Pause (Esc)", &paused);
        // if (!paused) {
        //     ImGui::BeginDisabled();
        //     ImGui::Button("Next Generation (Space)");
        //     ImGui::EndDisabled();
        // } else {
        //     if (ImGui::Button("Next Generation (Space)")) {
        //         nextGeneration();
        //     }
        // }
        // ImGui::Text("Up/Down: +1/-1");
        // ImGui::Text("Ctrl Up/Down: +5/-5\n");
        // ImGui::ColorEdit3("Background", reinterpret_cast<float*>(&backgroundColor));

        ImGui::Text(("Time passed: " + std::to_string((int)elapsedTime.count()) + " seconds").c_str());

        ImGui::PushItemWidth(25);
        static char widthBuf[3] = ""; ImGui::InputText(" Width (5-20)", widthBuf, 3, ImGuiInputTextFlags_CharsDecimal);
        static char heightBuf[3] = ""; ImGui::InputText(" Height (5-20)", heightBuf, 3, ImGuiInputTextFlags_CharsDecimal);
        ImGui::PopItemWidth();
        ImGui::PushItemWidth(35);
        static char bombsBuf[4] = ""; ImGui::InputText((" Bombs (5-" + std::to_string(std::max(std::atoi(widthBuf) * std::atoi(heightBuf), 5)) + ")").c_str(), bombsBuf, 4, ImGuiInputTextFlags_CharsDecimal);
        ImGui::PopItemWidth();
        if (ImGui::Button("Start")) {
            startNewGame(std::atoi(widthBuf), std::atoi(heightBuf), std::atoi(bombsBuf));
            LOG("PlayButton Pressed")
        }
        if (invalidInput) {
            ImGui::Text("Invalid Input");
        }

        ImGui::End();
        //ImGui::ShowDemoWindow();

        ImGui::Render();
        SDL_RenderClear(renderer);
        drawBoard();
        if (gameOver || gameWon) {
            drawBombs();
        }
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }
    return 0;
}

void Game::drawBoard() {
    for (int i = 0; i < gameMap.mapWidth; ++i) {
        for (int j = 0; j < gameMap.mapHeight; ++j) {
            if (gameMap.tiles[j][i].isFlagged) {
                if ((int)mouseX / TILE_SIZE == i && (int)mouseY / TILE_SIZE == j) {
                    SDL_RenderTexture(renderer, hoverFlagTexture, nullptr, &gameMap.tiles[j][i].position);
                } else {
                    SDL_RenderTexture(renderer, flagTexture, nullptr, &gameMap.tiles[j][i].position);
                }
            } else if(gameMap.tiles[j][i].isOpened) {
                if (gameMap.tiles[j][i].bombNeighbours) {
                    SDL_RenderTexture(renderer, numberTextures[gameMap.tiles[j][i].bombNeighbours - 1], nullptr, &gameMap.tiles[j][i].position);
                } else {
                    SDL_RenderTexture(renderer, openedTexture, nullptr, &gameMap.tiles[j][i].position);
                }

            } else {
                if ((int)mouseX / TILE_SIZE == i && (int)mouseY / TILE_SIZE == j) {
                    SDL_RenderTexture(renderer, hoverClosedTexture, nullptr, &gameMap.tiles[j][i].position);
                } else {
                    SDL_RenderTexture(renderer, closedTexture, nullptr, &gameMap.tiles[j][i].position);
                }
            }
        }
    }
}

void Game::drawBombs() {
    for (int i = 0; i < gameMap.mapWidth; ++i) {
        for (int j = 0; j < gameMap.mapHeight; ++j) {
            if(gameMap.tiles[j][i].hasBomb) {
                SDL_RenderTexture(renderer, bombTexture, nullptr, &gameMap.tiles[j][i].position);
            }
        }
    }
}

void Game::handleMouseClick(int tileX, int tileY, bool isLeftClick) {
    if (mouseX / TILE_SIZE > gameMap.mapWidth) {
        return;
    }
    if (isLeftClick) {
        if(firstLeftPress) {
            firstLeftPress = false;
            if (gameMap.tiles[tileY][tileX].hasBomb) {
                gameMap.tiles[tileY][tileX].hasBomb = false;

                while (true) {
                    int i = rand() % gameMap.mapWidth;
                    int j = rand() % gameMap.mapHeight;

                    if(!gameMap.tiles[j][i].hasBomb && tileX != i && tileY != j) {
                        gameMap.tiles[j][i].hasBomb = true;
                        break;
                    }
                }
                gameMap.countNeighbourBombs();
            }
        }
        if (!gameMap.tiles[tileY][tileX].isFlagged) {
            if (!gameMap.tiles[tileY][tileX].hasBomb) {
                if (gameMap.tiles[tileY][tileX].bombNeighbours) {
                    gameMap.tiles[tileY][tileX].isOpened = true;
                } else {
                    openTilesRecursive(tileX, tileY);
                }
            } else {
                gameOver = true;
                LOG("Game Over");
            }
        }
    } else { // rightClick
        if (!gameMap.tiles[tileY][tileX].isOpened && !firstLeftPress) {
            gameMap.tiles[tileY][tileX].isFlagged = !gameMap.tiles[tileY][tileX].isFlagged;
        }
    }
}

void Game::openTilesRecursive(int tileX, int tileY) {
    gameMap.tiles[tileY][tileX].isOpened = true;

    if (!gameMap.tiles[tileY][tileX].bombNeighbours) {
        if (tileX && tileY) {
            if (!gameMap.tiles[tileY - 1][tileX - 1].isOpened) {
                openTilesRecursive(tileX - 1, tileY - 1);
            }
        }
        if (tileX && tileY != gameMap.mapHeight - 1) {
            if (!gameMap.tiles[tileY + 1][tileX - 1].isOpened) {
                openTilesRecursive(tileX - 1, tileY + 1);
            }
        }
        if (tileX != gameMap.mapWidth - 1 && tileY != gameMap.mapHeight - 1) {
            if (!gameMap.tiles[tileY + 1][tileX + 1].isOpened) {
                openTilesRecursive(tileX + 1, tileY + 1);
            }
        }
        if (tileX != gameMap.mapWidth - 1 && tileY) {
            if (!gameMap.tiles[tileY - 1][tileX + 1].isOpened) {
                openTilesRecursive(tileX + 1, tileY - 1);
            }
        }
        if (tileX) {
            if (!gameMap.tiles[tileY][tileX - 1].isOpened) {
                openTilesRecursive(tileX - 1, tileY);
            }
        }
        if (tileY != gameMap.mapHeight - 1) {
            if (!gameMap.tiles[tileY + 1][tileX].isOpened) {
                openTilesRecursive(tileX, tileY + 1);
            }
        }
        if (tileX != gameMap.mapWidth - 1) {
            if (!gameMap.tiles[tileY][tileX + 1].isOpened) {
                openTilesRecursive(tileX + 1, tileY);
            }
        }
        if (tileY) {
            if (!gameMap.tiles[tileY - 1][tileX].isOpened) {
                openTilesRecursive(tileX, tileY - 1);
            }
        }
    }
}

bool Game::checkWin() {
    for (int i = 0; i < gameMap.mapWidth; ++i) {
        for (int j = 0; j < gameMap.mapHeight; ++j) {
            if (!gameMap.tiles[j][i].isOpened && !gameMap.tiles[j][i].hasBomb) {
                return false;
            }
        }
    }
    return true;
}