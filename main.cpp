#include "source/constant.h"
#include "source/Object.h"
#include "source/GameMap.h"
#include "source/Player.h"
#include "source/Enemy.h"
#include "source/Explosion.h"
#include "source/Text.h"
#include "source/Display.h"
#include "source/AssistDisplay.h"
Object g_background;
TTF_Font* font_time = NULL;

bool InitData(){
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if(ret < 0){ return false; }
    // sound
    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1){ success = false; }
    g_sound_bullet[0] = Mix_LoadWAV("sound//Fire1.wav");
    g_sound_exp = Mix_LoadWAV("sound//Explosion1.wav");
    g_sound_exp_player = Mix_LoadWAV("sound//Explosion1.wav");
    if(g_sound_bullet[0] == NULL || g_sound_exp == NULL || g_sound_exp_player == NULL){ success = false; }
    
    // window
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    g_window = SDL_CreateWindow("Game SDL",
                                SDL_WINDOWPOS_UNDEFINED, 
                                SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH, 
                                SCREEN_HEIGHT, 
                                SDL_WINDOW_SHOWN);
    if(g_window == NULL){
        success = false;

    } else {

        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if(g_screen == NULL){
            success = false;
        } else {
            SDL_SetRenderDrawColor(g_screen, 255, 255,255, 255);
            int imgFlags = IMG_INIT_PNG;
            if( ! ( IMG_Init(imgFlags) && imgFlags ) ){ success = false; }
        }

        if(TTF_Init() == -1){ success = false; }
        font_time = TTF_OpenFont("font//dlxfont_.ttf", TEXT_FONT);
        if(font_time == NULL){ success = false; }
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
            p_enemy->set_type_move(Enemy::STATIC);
            p_enemy->set_x_pos(1000 + i*500);
            p_enemy->set_y_pos(250);
            p_enemy->set_input_left(0);
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

    GameMap game_map;
    string mapStructure = "map/map01.dat";
    game_map.LoadMap((char*)mapStructure.c_str());
    game_map.LoadTiles(g_screen);

    Player p_player;
    p_player.LoadImg("img//player_right.png", g_screen);
    p_player.set_clips(); // hieu ung chuyen dong

    Display display_score;
    display_score.Init(g_screen);
    DisplayCoin display_coin;
    display_coin.Init(g_screen);
    display_coin.SetPos(SCREEN_WIDTH*0.5 - 300, 8);

    vector<Enemy*> enemies = MakeEnemyList();
    Explosion exp_enemy;
    bool eRet = exp_enemy.LoadImg("img//explode.png", g_screen);
    if(!eRet){ return -1; }
    exp_enemy.set_clip();

    Explosion exp_main;
    bool mRet = exp_main.LoadImg("img//explode.png", g_screen);
    if(!mRet){ return -1; }
    exp_main.set_clip();
    int num_eliminated = 0;

    Text time_count;
    time_count.SetColor(Text::WHITE_TEXT);
    Text score_game;
    score_game.SetColor(Text::WHITE_TEXT);
    UINT score_value = 0;
    Text coin_count;
    coin_count.SetColor(Text::WHITE_TEXT);

    bool is_quit = false;
    while(!is_quit){

        Uint32 frameStart = SDL_GetTicks();
        while(SDL_PollEvent(&g_event) != 0){
            if(g_event.type == SDL_QUIT){is_quit = true;}
            p_player.Handle_Input_Action(g_event, g_screen, g_sound_bullet);
        }

        SDL_SetRenderDrawColor(g_screen, 255, 255,255, 255);
        SDL_RenderClear(g_screen);
        g_background.Render(g_screen, NULL);
        Map map_data = game_map.getMap();
        
        p_player.HandleBullet(g_screen, map_data);
        p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
        p_player.DoPlayer(map_data);
        p_player.Show(g_screen);

        game_map.SetMap(map_data);
        game_map.DrawMap(g_screen);

        Geometry rectangle(0, 0, SCREEN_WIDTH, DISPLAY_HEIGHT);
        ColorDat color(36, 36, 36);
        AssistDisplay::RenderRectangle(rectangle, color, g_screen);
        Geometry outLine(OUTLINE_WIDTH, OUTLINE_WIDTH, SCREEN_WIDTH - 1, DISPLAY_HEIGHT - 2*OUTLINE_WIDTH);
        ColorDat color2(RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        AssistDisplay::RenderOutline(outLine, color2, g_screen);

        display_score.Show(g_screen);
        display_coin.Show(g_screen);

        // xu li: va cham voi player
        for(int i = 0; i < enemies.size(); i++){
            Enemy* p_threat = enemies.at(i);
            if(p_threat != NULL){
                p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
                p_threat->ImpMoveType(g_screen);
                p_threat->DoEnemy(map_data);
                p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT, map_data);
                p_threat->Show(g_screen);

                SDL_Rect rect_player = p_player.GetRectFrame();
                bool bCol1 = false;
                vector<Bullet*> tBullet_list = p_threat->get_bullet_list();
                for(int j=0; j<tBullet_list.size(); j++){
                    Bullet* pt_bullet = tBullet_list.at(j);
                    if(pt_bullet){
                        bCol1 = Collision::CheckCollision(pt_bullet->GetRect(), rect_player);
                        if(bCol1){
                            break;
                        }
                    }
                }

                SDL_Rect rect_threat = p_threat->GetRectFrame();
                bool bCol2 = Collision::CheckCollision(rect_player, rect_threat);
                if(bCol2 || bCol1){
                    int width_exp_main = exp_main.get_frame_width();
                    int height_exp_main = exp_main.get_frame_height();
                    for(int i=0; i<EXPLOSION_FRAME/2; i++){
                        int x_pos = p_player.GetRect().x + p_player.get_width_frame()*0.5 - width_exp_main*0.5;
                        int y_pos = p_player.GetRect().y + p_player.get_height_frame()*0.5 - height_exp_main*0.5;
                        exp_main.set_frame(i);
                        exp_main.SetRect(x_pos, y_pos);
                        exp_main.Show(g_screen);
                        SDL_RenderPresent(g_screen);
                    }
                    if(bCol2){
                        p_threat->Free();
                        enemies.erase(enemies.begin() + i);
                        score_value++;
                    }
                    Mix_PlayChannel(-1, g_sound_exp_player, 0);
                    num_eliminated++;
                    if(num_eliminated <= LIFE){
                        p_player.SetRect(0, 0);
                        p_player.set_comeback_time(COMEBACK_TIME);
                        display_score.Decrease();
                        display_score.Render(g_screen);
                        continue;
                    } else{
                        SDL_Delay(GAMEOVER);
                        if(MessageBox(NULL, "GAME OVER", "Oops", MB_OK | MB_ICONSTOP) == IDOK){
                            p_threat->Free();
                            close();
                            SDL_Quit();
                            return 0;
                        }
                    }
                }
            }
        }

        // xu li: dan va cham enemy
        int frame_exp_width = exp_enemy.get_frame_width();
        int frame_exp_height = exp_enemy.get_frame_height();
        vector<Bullet*> bullet_arr = p_player.get_bullet_list();
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
                        bool bCol = Collision::CheckCollision(bRect, eRect);
                        if(bCol){
                            score_value++;
                            for(int i=0; i<EXPLOSION_FRAME; i++){
                                // ban dau vi tri o mep => tru di 1 nua frame
                                int x_pos = p_bullet->GetRect().x - frame_exp_width*0.5;
                                int y_pos = p_bullet->GetRect().y - frame_exp_height*0.5;
                                exp_enemy.set_frame(i);
                                exp_enemy.SetRect(x_pos, y_pos);
                                exp_enemy.Show(g_screen);
                            }

                            Mix_PlayChannel(-1, g_sound_exp, 0);
                            p_bullet->Free();
                            p_player.RemoveBullet(i);
                            enemy->Free();
                            enemies.erase(enemies.begin() + j);
                        }
                    }
                }
            }
        }
        
        // show game time
        string str_time = "Time: ";
        Uint32 time_val = SDL_GetTicks()/1000;
        Uint32 val_time = TIME_TOTAL - time_val;
        if(val_time <= 0){
            if(MessageBox(NULL, "GAME OVER", "Oops", MB_OK | MB_ICONSTOP) == IDOK){
                is_quit = true;
                break;
            }

        } else {
            string str_val = to_string(val_time);
            str_time += str_val;
            if(val_time < TIME_TOTAL/2) { time_count.SetColor(Text::RED_TEXT); }
            time_count.SetText(str_time);
            time_count.LoadFromRenderText(font_time, g_screen);
            time_count.RenderText(g_screen, SCREEN_WIDTH - 200, TEXT_FONT);
        }

        // show score
        string strScore("Score: ");
        strScore = strScore + to_string(score_value) + "/" + to_string(2*ENEMY_OBJECT);

        if(score_value == 2*ENEMY_OBJECT){
            enemies.back()->Free();
            enemies.pop_back();
            SDL_Delay(GAMEOVER);
            if(MessageBox(NULL, "YOU WIN", "Congratulation", MB_OK | MB_ICONINFORMATION) == IDOK){
                is_quit = true;
                break;
            }
        } else if (score_value > ENEMY_OBJECT){
            score_game.SetColor(Text::RED_TEXT);
        }

        score_game.SetText(strScore);
        score_game.LoadFromRenderText(font_time, g_screen);
        score_game.RenderText(g_screen, SCREEN_WIDTH*0.5 - 50, TEXT_FONT);

        // show coin
        int coin_count_val = p_player.get_coin_count();
        string str_coin_count = to_string(coin_count_val);
        coin_count.SetText(str_coin_count);
        coin_count.LoadFromRenderText(font_time, g_screen);
        coin_count.RenderText(g_screen, SCREEN_WIDTH*0.5 - 250, TEXT_FONT);

        SDL_RenderPresent(g_screen);
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime){SDL_Delay(frameDelay - frameTime);}
    }

    for(int i=0; i<enemies.size(); i++){
        Enemy* enemy = enemies.at(i);
        enemy->Free();
        enemies.erase(enemies.begin() + i);
    }
    close();
    return 0;
}