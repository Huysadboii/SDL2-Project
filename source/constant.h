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

// audio
#define BULLET_SOUND 1
static Mix_Chunk* g_sound_bullet[BULLET_SOUND];
static Mix_Chunk* g_sound_exp = NULL;
static Mix_Chunk* g_sound_exp_player = NULL;
static Mix_Chunk* g_sound_life = NULL;
static Mix_Chunk* g_theme = NULL;

//screen
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640
#define SCREEN_BPP 32
#define DISPLAY_HEIGHT 50
#define OUTLINE_WIDTH 1
#define COLOR_KEY_R 167
#define COLOR_KEY_G 175
#define COLOR_KEY_B 180
#define RENDER_DRAW_COLOR 255

// text pos in gameplay
#define TEXT_FONT 20
#define POS_OF_COIN 340
#define POS_OF_TIME 1080
#define POS_OF_TEXT 250
#define POS_OF_SCORE 800

// text pos in menu
#define POS_OF_MENU 330
#define OVER_SCORE_POS 500
#define WIN_SCORE_POS 400
#define POS_OF_INS 20
// height menu text
#define TEXT_DIS 40
#define SCORE_HEIGHT 150
#define MENU_HEIGHT 260
#define INS_HEIGHT 20
#define ESCAPE_HEIGHT 600
// menu lines
#define MENU_LINE 3
#define OVER_LINE 2
#define INSTRUCTIONS_LINE 9

// gameplay time
#define FPS 60
#define TIME_TOTAL 180
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

// enemy
#define ENEMY_OBJECT_GUNNER 30
#define ENEMY_OBJECT_STATIC 30
#define TOTAL_ENEMY ENEMY_OBJECT_GUNNER + ENEMY_OBJECT_STATIC
#define ENEMY_SPEED 2
#define MAX_BULLET_DIS 500

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