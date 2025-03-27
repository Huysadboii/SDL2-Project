#ifndef CONSTANT_H_
#define CONSTANT_H_
using namespace std;

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

#define BULLET_SOUND 1
static Mix_Chunk* g_sound_bullet[BULLET_SOUND];
static Mix_Chunk* g_sound_exp = NULL;
static Mix_Chunk* g_sound_exp_player = NULL;
static Mix_Chunk* g_sound_life = NULL;

//screen
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640
#define SCREEN_BPP 32
#define TEXT_FONT 20
#define COLOR_KEY_R 167
#define COLOR_KEY_G 175
#define COLOR_KEY_B 180
#define RENDER_DRAW_COLOR 255
#define DISPLAY_HEIGHT 40
#define OUTLINE_WIDTH 1

// gameplay time
#define FPS 60
#define TIME_TOTAL 1000 //120
#define GAMEOVER 500
#define FIRING_DELAY 300

// player's physics
#define GRAVITY 1
#define PLAYER_SPEED 11
#define PLAYER_JUMP_VAL 20
#define MAX_FALL_SPEED 10
#define DECLINE_BULLET 0.25 // increase to decline bullet

// player's resurection
#define RESPAWN 9
#define COMEBACK_TIME 100
#define LIFE 2
#define COIN_PER_LIFE 10

//map properties
#define TILE_SIZE 64
#define MAX_MAP_X 400
#define MAX_MAP_Y 10
#define MAX_TILES 10
#define BLANK_TILE 0
#define COIN_TILE 4
#define FLAG_TILE 8

// frame
#define EXPLOSION_FRAME 8
#define PLAYER_FRAME 8
#define ENEMY_FRAME 8

// enemy
#define ENEMY_TYPE 2
#define ENEMY_SPEED 3
#define ENEMY_OBJECT 20
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

#endif