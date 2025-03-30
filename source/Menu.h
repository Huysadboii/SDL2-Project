#ifndef MENU_H
#define MENU_H

#include "constant.h"

class Menu {
    public:
        enum GameState { MENU, PLAYING, GAME_OVER, VICTORY, INSTRUCTIONS };
        GameState getCurrentState() const { return currentState; }
        Menu();
        ~Menu();

        void handleMenuEvents(SDL_Event& event, GameState& state, bool& running);
        void renderMenu(SDL_Renderer* renderer);

        void handleGameOverEvents(SDL_Event& event, GameState& state, bool& running);
        void renderGameOver(SDL_Renderer* renderer);

        void renderVictory(SDL_Renderer* renderer);

        void handleInstructionsEvents(SDL_Event& event, GameState& state, bool& running);
        void renderInstructions(SDL_Renderer* renderer);

    private:
        GameState currentState;
        SDL_Texture* loadTexture(std::string path, SDL_Renderer* renderer);

        SDL_Texture* menuTexture;
        SDL_Texture* gameOverTexture;
        SDL_Texture* victoryTexture;
        SDL_Texture* instructionsTexture;
};

#endif
