#include "Menu.h"

Menu::Menu() {
    currentState = MENU;
    menuTexture = nullptr;
    gameOverTexture = nullptr;
    victoryTexture = nullptr;
    instructionsTexture = nullptr;
}

Menu::~Menu() {
    if (menuTexture) SDL_DestroyTexture(menuTexture);
    if (gameOverTexture) SDL_DestroyTexture(gameOverTexture);
    if (victoryTexture) SDL_DestroyTexture(victoryTexture);
    if (instructionsTexture) SDL_DestroyTexture(instructionsTexture);
}

SDL_Texture* Menu::loadTexture(string path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface) {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void Menu::handleMenuEvents(SDL_Event& event, GameState& state, bool& running) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_RETURN) {
                state = PLAYING;
            }
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
            if (event.key.keysym.sym == SDLK_i) {
                state = INSTRUCTIONS;
            }
        }
    }
}

void Menu::renderMenu(SDL_Renderer* renderer) {
    if (!menuTexture) {
        menuTexture = loadTexture("img//menu.png", renderer);
    }
    SDL_RenderCopy(renderer, menuTexture, NULL, NULL);
}

void Menu::handleGameOverEvents(SDL_Event& event, GameState& state, bool& running) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_RETURN) {
                state = MENU;
            }
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }
    }
}

void Menu::renderGameOver(SDL_Renderer* renderer) {
    if (!gameOverTexture) {
        gameOverTexture = loadTexture("img//game_over.png", renderer);
    }
    SDL_RenderCopy(renderer, gameOverTexture, NULL, NULL);
}

void Menu::renderVictory(SDL_Renderer* renderer) {
    if (!victoryTexture) {
        victoryTexture = loadTexture("img//victory.png", renderer);
    }
    SDL_RenderCopy(renderer, victoryTexture, NULL, NULL);
}

void Menu::handleInstructionsEvents(SDL_Event& event, GameState& state, bool& running) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                state = MENU;
            }
        }
    }
}

void Menu::renderInstructions(SDL_Renderer* renderer) {
    if (!instructionsTexture) {
        instructionsTexture = loadTexture("img//instruction.png", renderer);
    }
    SDL_RenderCopy(renderer, instructionsTexture, NULL, NULL);
}