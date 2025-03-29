#ifndef MENU_H
#define MENU_H

#include "constant.h"

class Menu {
    public:
        enum GameState { MENU, PLAYING, GAME_OVER, VICTORY };

        Menu();
        ~Menu();

        void handleMenuEvents(SDL_Event& event, GameState& state, bool& running);
        void renderMenu(SDL_Renderer* renderer);

        void handleGameOverEvents(SDL_Event& event, GameState& state, bool& running);
        void renderGameOver(SDL_Renderer* renderer);
        void renderVictory(SDL_Renderer* renderer);


        GameState getCurrentState() const { return currentState; }

    private:
        GameState currentState;
        SDL_Texture* menuTexture;
        SDL_Texture* gameOverTexture;

        SDL_Texture* loadTexture(std::string path, SDL_Renderer* renderer);
};

#endif
