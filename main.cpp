#include "source/constant.h"
#include "source/Object.h"
#include "source/GameMap.h"
#include "source/Player.h"
#include "source/Enemy.h"
#include "source/Explosion.h"
#include "source/Text.h"
#include "source/Display.h"
#include "source/AssistDisplay.h"
#include "source/Menu.h"
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
    g_sound_exp_player = Mix_LoadWAV("sound//Laser.wav");
    g_sound_life = Mix_LoadWAV("sound//two_beep.wav");
    g_theme = Mix_LoadWAV("sound//Action.wav");
    if(g_sound_bullet[0] == NULL || g_sound_exp == NULL || g_sound_exp_player == NULL || 
            g_sound_life == NULL || g_theme == NULL)
    { success = false; }
    
    // window
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    g_window = SDL_CreateWindow("Brave Soldier",
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
    TTF_Quit();
    SDL_Quit();
}
vector<Enemy*> MakeEnemyList(){
    vector<Enemy*> list_enemies;

    Enemy* dynamic_enemy = new Enemy[ENEMY_OBJECT_GUNNER];
    for(int i=0; i<ENEMY_OBJECT_GUNNER; i++){
        Enemy* p_enemy = dynamic_enemy + i;
        if(p_enemy != NULL){
            p_enemy->LoadImg("img//threat_left.png", g_screen);
            p_enemy->set_clips();
            p_enemy->set_type_move(Enemy::MOVE_IN_SPACE);
            p_enemy->set_x_pos(1000 + i*800);
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

    Enemy* enemy_objects = new Enemy[ENEMY_OBJECT_STATIC];
    for(int i = 0; i < ENEMY_OBJECT_STATIC; i++){
        Enemy* p_enemy = enemy_objects + i;
        if(p_enemy != NULL){
            p_enemy->LoadImg("img//threat_level.png", g_screen);
            p_enemy->set_clips();
            p_enemy->set_type_move(Enemy::STATIC);
            p_enemy->set_x_pos(10000 + i*500);
            p_enemy->set_y_pos(250);
            p_enemy->set_input_left(0);
            Bullet* p_bullet = new Bullet();
            p_enemy->InitBullet(p_bullet, g_screen, 2);
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

    string mapStructure = "map/map01.dat";
    bool isMusicPlaying = false;
    UINT score_value;
    Text time_count, score_game, coin_count;
    Menu menu;
    Menu::GameState currentState = menu.getCurrentState();

    // menu
    Text menu_texts[MENU_LINE];
    string menu_text_strings[MENU_LINE] = {
        "Press Enter to start the game",
        "Press I for how to play",
        "Press Esc to quit the game"
    };
    for (int i = 0; i < MENU_LINE; ++i) {
        menu_texts[i].SetColor(Text::WHITE_TEXT);
        menu_texts[i].SetText(menu_text_strings[i]);
        menu_texts[i].LoadFromRenderText(font_time, g_screen);
    }

    // game over && victory
    Text over_texts[OVER_LINE];
    string over_text_strings[OVER_LINE] = {
        "Press Enter to return to menu",
        "Press Esc to quit the game"
    };
    for (int i = 0; i < OVER_LINE; ++i) {
        over_texts[i].SetColor(Text::WHITE_TEXT);
        over_texts[i].SetText(over_text_strings[i]);
        over_texts[i].LoadFromRenderText(font_time, g_screen);
    }
    Text over_score;
    over_score.SetColor(Text::WHITE_TEXT);
    Text win_score;
    win_score.SetColor(Text::WHITE_TEXT);
    
    // instructions
    Text instructions[INSTRUCTIONS_LINE];
    string instruction_texts[INSTRUCTIONS_LINE] = {
        "CONTROL: ",
        " * Press A/D to move left/right",
        " * Press SPACE to jump",
        " * Click LEFT MOUSE BUTTON to fire",
        "RULE: ",
        " - You have "+to_string(LIFE)+" lives and "+to_string(TIME_TOTAL)+" seconds to finish the game",
        " - You can earn extra life by collecting coins",
        " - Eliminate enemy to gain score",
        " ! Reach the flag to win the game"
    };
    for (int i = 0; i < INSTRUCTIONS_LINE; ++i) {
        instructions[i].SetColor(Text::WHITE_TEXT);
        instructions[i].SetText(instruction_texts[i]);
        instructions[i].LoadFromRenderText(font_time, g_screen);
    }
    Text esc_instructions;
    esc_instructions.SetColor(Text::WHITE_TEXT);
    esc_instructions.SetText("Press Esc to return to menu");
    esc_instructions.LoadFromRenderText(font_time, g_screen);

    bool running = true;
    while(running)
    {
        Uint32 frameStart = SDL_GetTicks();
        if ((currentState == Menu::MENU || currentState == Menu::INSTRUCTIONS) && !isMusicPlaying) {
            Mix_PlayChannel(-1, g_theme, 0);
            isMusicPlaying = true;
        } else if (currentState != Menu::MENU && currentState != Menu::INSTRUCTIONS && isMusicPlaying) {
            Mix_HaltChannel(-1);
            isMusicPlaying = false;
        }
        switch ( currentState )
        {
            case Menu::MENU:
                menu.handleMenuEvents(g_event, currentState, running);
                SDL_RenderClear(g_screen);
                menu.renderMenu(g_screen);

                for (int i = 0; i < MENU_LINE; ++i) {
                    menu_texts[i].RenderText(g_screen, POS_OF_MENU, MENU_HEIGHT + i * TEXT_DIS);
                }

                SDL_RenderPresent(g_screen);
                break;

            case Menu::INSTRUCTIONS:
                menu.handleInstructionsEvents(g_event, currentState, running);
                SDL_RenderClear(g_screen);
                menu.renderInstructions(g_screen);

                for (int i = 0; i < INSTRUCTIONS_LINE; ++i) {
                    instructions[i].RenderText(g_screen, POS_OF_INS, INS_HEIGHT + i * TEXT_DIS);
                }
                esc_instructions.RenderText(g_screen, POS_OF_INS, ESCAPE_HEIGHT);

                SDL_RenderPresent(g_screen);
                break;

            case Menu::GAME_OVER:
                menu.handleGameOverEvents(g_event, currentState, running);
                SDL_RenderClear(g_screen);
                menu.renderGameOver(g_screen);

                over_score.RenderText(g_screen, OVER_SCORE_POS, SCORE_HEIGHT);
                for (int i = 0; i < OVER_LINE; ++i) {
                    over_texts[i].RenderText(g_screen, POS_OF_MENU, MENU_HEIGHT + i * TEXT_DIS);
                }
                
                SDL_RenderPresent(g_screen);
                break;

            case Menu::VICTORY:
                menu.handleGameOverEvents(g_event, currentState, running);
                SDL_RenderClear(g_screen);
                menu.renderVictory(g_screen);

                win_score.RenderText(g_screen, WIN_SCORE_POS, SCORE_HEIGHT);
                for (int i = 0; i < 2; ++i) {
                    over_texts[i].RenderText(g_screen, POS_OF_MENU, MENU_HEIGHT + i * TEXT_DIS);
                }

                SDL_RenderPresent(g_screen);
                break;

            case Menu::PLAYING:

            bool is_quit = false;
            int life_added = 0;
            int num_eliminated = 0;
            Uint32 start_time = 0;
            score_value = 0;

            Player p_player;
            GameMap game_map;
            Display display_life;
            DisplayCoin display_coin;
            Explosion exp_enemy, exp_main;

            game_map.LoadMap((char*)mapStructure.c_str());
            game_map.LoadTiles(g_screen);
        
            p_player.LoadImg("img//player_right.png", g_screen);
            p_player.set_clips();
        
            display_life.Init(g_screen);
            display_coin.Init(g_screen);
            display_coin.SetPos(POS_OF_COIN, 8);
        
            vector<Enemy*> enemies = MakeEnemyList();
            bool eRet = exp_enemy.LoadImg("img//explode.png", g_screen);
            if(!eRet){ return -1; }
            exp_enemy.set_clip();
        
            bool mRet = exp_main.LoadImg("img//explode.png", g_screen);
            if(!mRet){ return -1; }
            exp_main.set_clip();
        
            time_count.SetColor(Text::WHITE_TEXT);
            score_game.SetColor(Text::WHITE_TEXT);
            coin_count.SetColor(Text::WHITE_TEXT);
            
            while(!is_quit){
                int framePlaying = SDL_GetTicks();
                while(SDL_PollEvent(&g_event) != 0){
                    if(g_event.type == SDL_QUIT){
                        is_quit = true;
                        running = false;
                        break;
                    }
                    p_player.Handle_Input_Action(g_event, g_screen, g_sound_bullet);
                }
        
                SDL_SetRenderDrawColor(g_screen, 255, 255,255, 255);
                SDL_RenderClear(g_screen);
                g_background.Render(g_screen, NULL);
                Map map_data = game_map.getMap();
        
                p_player.HandleBullet(g_screen, map_data);
                p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
                p_player.DoPlayer(map_data);
                if(p_player.FinishMap(map_data)){
                    SDL_Delay(GAMEOVER);
                    string win_score_text = "YOU WIN WITH THE SCORE: " + to_string(score_value);
                    win_score.SetText(win_score_text);
                    win_score.LoadFromRenderText(font_time, g_screen);
                    currentState = Menu::VICTORY;
                    is_quit = true;
                    break;
                }
                if(p_player.get_falled() && p_player.get_stop_count() == false){
                    Mix_PlayChannel(-1, g_sound_exp_player, 0);
                    num_eliminated++;
                    display_life.Decrease();
                    display_life.Render(g_screen);
                    if(num_eliminated < LIFE){
                        p_player.SetRect(0, 0);
                        p_player.set_comeback_time(COMEBACK_TIME);
                        continue;
                    } else{
                        SDL_Delay(GAMEOVER);
                        string over_score_text = "YOUR SCORE: " + to_string(score_value);
                        over_score.SetText(over_score_text);
                        over_score.LoadFromRenderText(font_time, g_screen);
                        currentState = Menu::GAME_OVER;
                        is_quit = true;
                        break;
                    }
                }
                
                p_player.Show(g_screen);
                game_map.SetMap(map_data);
                game_map.DrawMap(g_screen);
        
                Geometry rectangle(0, 0, SCREEN_WIDTH, DISPLAY_HEIGHT);
                ColorDat color(36, 36, 36);
                AssistDisplay::RenderRectangle(rectangle, color, g_screen);
                Geometry outLine(OUTLINE_WIDTH, OUTLINE_WIDTH, SCREEN_WIDTH - 1, DISPLAY_HEIGHT - 2*OUTLINE_WIDTH);
                ColorDat color2(RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
                AssistDisplay::RenderOutline(outLine, color2, g_screen);
        
                display_life.Show(g_screen);
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
                            for(int k=0; k<EXPLOSION_FRAME/2; k++){
                                int x_pos = p_player.GetRect().x + p_player.get_width_frame()*0.5 - width_exp_main*0.5;
                                int y_pos = p_player.GetRect().y + p_player.get_height_frame()*0.5 - height_exp_main*0.5;
                                exp_main.set_frame(k);
                                exp_main.SetRect(x_pos, y_pos);
                                exp_main.Show(g_screen);
                                SDL_RenderPresent(g_screen);
                            }
                            Mix_PlayChannel(-1, g_sound_exp_player, 0);
        
                            if(bCol2){
                                p_threat->Free();
                                enemies.erase(enemies.begin() + i);
                                score_value++;
                            }
                            num_eliminated++;
                            display_life.Decrease();
                            display_life.Render(g_screen);
        
                            if(num_eliminated < LIFE){
                                p_player.SetRect(0, 0);
                                p_player.set_comeback_time(COMEBACK_TIME);
                                continue;
                            } else{
                                SDL_Delay(GAMEOVER);
                                string over_score_text = "YOUR SCORE: " + to_string(score_value);
                                over_score.SetText(over_score_text);
                                over_score.LoadFromRenderText(font_time, g_screen);
                                is_quit = true;
                                currentState = Menu::GAME_OVER;
                                break;
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
                                    for(int k=0; k<EXPLOSION_FRAME; k++){
                                        // ban dau vi tri o mep => tru di 1 nua frame
                                        int x_pos = p_bullet->GetRect().x - frame_exp_width*0.5;
                                        int y_pos = p_bullet->GetRect().y - frame_exp_height*0.5;
                                        exp_enemy.set_frame(k);
                                        exp_enemy.SetRect(x_pos, y_pos);
                                        exp_enemy.Show(g_screen);
                                    }
                                    
                                    score_value++;
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
                
                // show time
                if(start_time == 0){ start_time = SDL_GetTicks(); }
                string str_time = "Time: ";
                Uint32 time_val =( SDL_GetTicks() - start_time)/1000;
                Uint32 val_time = TIME_TOTAL - time_val;
                if(val_time <= 0){
                    SDL_Delay(GAMEOVER);
                    string over_score_text = "YOUR SCORE: " + to_string(score_value);
                    over_score.SetText(over_score_text);
                    over_score.LoadFromRenderText(font_time, g_screen);
                    currentState = Menu::GAME_OVER;
                    is_quit = true;
                    break;
                } else {
                    string str_val = to_string(val_time);
                    str_time += str_val;
                    if(val_time < TIME_TOTAL/2) { time_count.SetColor(Text::RED_TEXT); }
                    time_count.SetText(str_time);
                    time_count.LoadFromRenderText(font_time, g_screen);
                    time_count.RenderText(g_screen, POS_OF_TIME, TEXT_FONT);
                }
        
                // show score
                string strScore("Score: ");
                strScore = strScore + to_string(score_value) + "/";
                strScore += to_string(TOTAL_ENEMY);
                if (score_value > (TOTAL_ENEMY)/2)
                    { score_game.SetColor(Text::RED_TEXT); }
                score_game.SetText(strScore);
                score_game.LoadFromRenderText(font_time, g_screen);
                score_game.RenderText(g_screen, POS_OF_SCORE, TEXT_FONT);
        
                // show coin
                int coin_count_val = p_player.get_coin_count() - life_added*COIN_PER_LIFE;
                string str_coin_count = to_string(coin_count_val) + "/" + to_string(COIN_PER_LIFE);
                if(coin_count_val >= COIN_PER_LIFE){
                    life_added++;
                    num_eliminated--;
                    display_life.Increase();
                    display_life.Render(g_screen);
                    Mix_PlayChannel(-1, g_sound_life, 0);
                }
                coin_count.SetText(str_coin_count);
                coin_count.LoadFromRenderText(font_time, g_screen);
                coin_count.RenderText(g_screen, SCREEN_WIDTH*0.5 - 250, TEXT_FONT);
        
                SDL_RenderPresent(g_screen);
                Uint32 frameTimePlay = SDL_GetTicks() - framePlaying;
                if(frameDelay > frameTimePlay){SDL_Delay(frameDelay - frameTimePlay);}
            }
            break;
        }
    
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime){SDL_Delay(frameDelay - frameTime);}
    }

    close();
    return 0;
}