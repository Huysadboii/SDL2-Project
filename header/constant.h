#ifndef CONSTANT_H_
#define CONSTANT_H_

#include <windows.h>
#include <string>
#include <vector>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

//screen
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640
#define SCREEN_BPP 32
// color code to neutralize character
#define COLOR_KEY_R 167
#define COLOR_KEY_G 175
#define COLOR_KEY_B 180
#define RENDER_DRAW_COLOR 255
#define FPS 60
//map properties
#define TILE_SIZE 64
#define MAX_MAP_X 400
#define MAX_MAP_Y 10
#define MAX_TILES 10 // number of block's type
#define BLANK_TILE 0
#define COIN_TILE 4
#define EXPLOSION_FRAME 8
// properties for player
#define GRAVITY 1
#define PLAYER_SPEED 12 // also bullet speed
#define PLAYER_JUMP_VAL 20

#define MAX_FALL_SPEED 10
#define COMEBACK_TIME 60
#define DECLINE_BULLET 0.25 // increase to decline bullet
// enemy
#define PLAYER_FRAME 8
#define ENEMY_FRAME 8
#define ENEMY_SPEED 5
#define ENEMY_OBJECT 20 // number of enemy
#define MAX_BULLET_DIS 400

struct Map{
    int start_x_;
    int start_y_;

    int max_x_;
    int max_y_;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    char* file_name_;
};

struct Input{
    int left_;
    int right_;
    int up_;
    int down_;
    int jump_;
};

namespace SDLconstant{
    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
}


#endif