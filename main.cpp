#include "header/constant.h"
#include "header/BaseObject.h"
#include "header/GameMap.h"
#include "header/Player.h"
#include "header/Enemy.h"
using namespace std;
BaseObject g_background;

// initialize SDL environment
bool InitData(){
    bool success = true;
    // set up initial environment
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if(ret < 0){ 
        return false;
    }
    // set up quality of the image
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    // create window
    g_window = SDL_CreateWindow(
        "Game SDL",
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, 
        SCREEN_HEIGHT, 
        SDL_WINDOW_SHOWN
    );
    if(g_window == NULL){
        success = false;
    } else {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if(g_screen == NULL){
            success = false;
        } else {
            SDL_SetRenderDrawColor(
                g_screen, 
                RENDER_DRAW_COLOR, 
                RENDER_DRAW_COLOR,
                RENDER_DRAW_COLOR, 
                RENDER_DRAW_COLOR
            );
            int imgFlags = IMG_INIT_PNG;
            if( ! ( IMG_Init(imgFlags) && imgFlags ) ){
                success = false;
            }
        }
    }
    return success;
}
bool loadBackground(){
    bool ret = g_background.LoadImg("img//background3.png", g_screen);
    return ret;
}
void close(){
    g_background.Free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}
vector<Enemy*> MakeEnemyList(){
    vector<Enemy*> list_enemies;

    Enemy* dynamic_enemy = new Enemy[ENEMY_OBJECT];
    for(int i=0; i<ENEMY_OBJECT; i++){
        Enemy* p_enemy = dynamic_enemy + i;
        if(p_enemy != NULL){
            p_enemy->LoadImg("img//threat_left.png", g_screen);
            p_enemy->set_clips();
            p_enemy->set_type_move(Enemy::MOVE_IN_SPACE);
            p_enemy->set_x_pos(700 + i*1200);
            p_enemy->set_y_pos(200);
            int pos1 = p_enemy->get_x_pos() - 60;
            int pos2 = p_enemy->get_x_pos() + 60;
            p_enemy->set_animation_pos(pos1, pos2);
            p_enemy->set_input_left(1);
            Bullet* p_bullet = new Bullet();
            p_enemy->InitBullet(p_bullet, g_screen, 2);
            list_enemies.push_back(p_enemy);
        }
    }

    Enemy* enemy_objects = new Enemy[ENEMY_OBJECT];
    for(int i = 0; i < ENEMY_OBJECT; i++){
        Enemy* p_enemy = enemy_objects + i;
        if(p_enemy != NULL){
            p_enemy->LoadImg("img//threat_level.png", g_screen);
            p_enemy->set_clips();
            p_enemy->set_x_pos(1000 + i*500);
            p_enemy->set_y_pos(250);
            p_enemy->set_type_move(Enemy::STATIC);
            p_enemy->set_input_left(0);
            Bullet* p_bullet = new Bullet();
            p_enemy->InitBullet(p_bullet, g_screen, 1);
            list_enemies.push_back(p_enemy);
        }
    }

    return list_enemies;
}

int main(int argc, char* argv[])
{
    int frameDelay = 1000 / FPS;
    if(InitData() == false){ return -1; }
    if(loadBackground() == false){ return -1; }

    GameMap game_map; // create game map object
    string mapStructure = "map/map01.dat";
    game_map.LoadMap((char*)mapStructure.c_str());
    game_map.LoadTiles(g_screen);

    Player player;
    player.LoadImg("img//player_right.png", g_screen);
    player.set_clips(); // hieu ung chuyen dong

    vector<Enemy*> enemies = MakeEnemyList();

    bool is_quit = false;
    while(!is_quit){

        Uint32 frameStart = SDL_GetTicks();

        while(SDL_PollEvent(&g_event) != 0){
            if(g_event.type == SDL_QUIT){
                is_quit = true;
            }
            player.Handle_Input_Action(g_event, g_screen);
        }

        SDL_SetRenderDrawColor(
            g_screen, 
            RENDER_DRAW_COLOR, 
            RENDER_DRAW_COLOR,
            RENDER_DRAW_COLOR, 
            RENDER_DRAW_COLOR
        ); 
        SDL_RenderClear(g_screen);
        g_background.Render(g_screen, NULL);
        
        Map map_data = game_map.getMap();
        
        player.HandleBullet(g_screen);
        player.SetMapXY(map_data.start_x_, map_data.start_y_);
        player.DoPlayer(map_data);
        player.Show(g_screen);

        game_map.SetMap(map_data);
        game_map.DrawMap(g_screen);

        for(int i = 0; i < enemies.size(); i++){
            Enemy* enemy = enemies.at(i);
            if(enemy != NULL){
                enemy->SetMapXY(map_data.start_x_, map_data.start_y_);
                enemy->ImpMoveType(g_screen);
                enemy->DoEnemy(map_data);
                enemy->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
                enemy->Show(g_screen);
            }
        }

        vector<Bullet*> bullet_arr = player.get_bullet_list();
        for(int i = 0; i < bullet_arr.size(); i++){
            Bullet* p_bullet = bullet_arr.at(i);

            if(p_bullet != NULL){
                for(int j=0; j<enemies.size(); j++){
                    Enemy* enemy = enemies.at(j);
                    
                    if(enemy != NULL){
                        SDL_Rect eRect;
                        eRect.x = enemy->GetRect().x;
                        eRect.y = enemy->GetRect().y;
                        eRect.w = enemy->get_width_frame();
                        eRect.h = enemy->get_height_frame();

                        SDL_Rect bRect = p_bullet->GetRect();
                        bool bCol = SDLconstant::CheckCollision(bRect, eRect);
                        if(bCol){
                            player.RemoveBullet(i);
                            enemy->Free();
                            enemies.erase(enemies.begin() + j);
                        }
                    }
                }
            }
        }
        
        SDL_RenderPresent(g_screen);
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }

    }

    for(int i=0; i<enemies.size(); i++){
        Enemy* enemy = enemies.at(i);
        delete enemy;
    }
    enemies.clear();
    close();
    return 0;
}