#include "GameMap.h"

void GameMap::LoadMap(char* name){

    FILE* fp = NULL;
    fopen_s(&fp, name, "rb");
    if(fp == NULL){
        return;
    }
    
    game_map_.max_x_ = 0;
    game_map_.max_y_ = 0;

    for(int i = 0; i < MAX_MAP_Y; i++){
        for(int j = 0; j < MAX_MAP_X; j++){
            fscanf(fp, "%d", &game_map_.tile[i][j]);
            int val = game_map_.tile[i][j];
            if(val > 0){
                if(j > game_map_.max_x_){
                    game_map_.max_x_ = j;
                }
                if(i > game_map_.max_y_){
                    game_map_.max_y_ = i;
                }
            }
        }
    }
    game_map_.max_x_ = (game_map_.max_x_ + 1) * TILE_SIZE;
    game_map_.max_y_ = (game_map_.max_y_ + 1) * TILE_SIZE;
    game_map_.start_x_ = 0;
    game_map_.start_y_ = 0;
    game_map_.file_name_ = name;
    fclose(fp);
}

// load hinh anh cho tilemat
void GameMap::LoadTiles(SDL_Renderer* screen){

    char file_img[30];
    FILE* fp = NULL;
    
    for(int i = 0; i < MAX_TILES; i++){
        sprintf_s(file_img, "map/%d.png", i);
        fopen_s(&fp, file_img, "rb");
        if(fp == NULL){
            continue;
        }
        fclose(fp);
        tile_mat[i].LoadImg(file_img, screen);
    }
}

// fill hinh anh vao cac o vuong
void GameMap::DrawMap(SDL_Renderer* screen){
    int x1=0; //fill tu dau den dau
    int x2=0;
    int y1=0;
    int y2=0;
    
    int map_x = 0;
    int map_y = 0;
    
    map_x = game_map_.start_x_ / TILE_SIZE;
    x1 = (game_map_.start_x_ % TILE_SIZE) *-1 ;
    x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

    map_y = game_map_.start_y_ / TILE_SIZE;
    y1 = (game_map_.start_y_ % TILE_SIZE) *-1 ;
    y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);

    //fill
    for(int i = y1; i < y2; i += TILE_SIZE){

        map_x = game_map_.start_x_ / TILE_SIZE;

        for(int j = x1; j < x2; j += TILE_SIZE){
            int val = game_map_.tile[map_y][map_x];
            if(val > 0){
                tile_mat[val].SetRect(j , i );
                tile_mat[val].Render(screen);
            }

            map_x++;
        }

        map_y++;
    }
}

bool Collision::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2){
    int left_a = object1.x;
    int right_a = object1.x + object1.w;
    int top_a = object1.y;
    int bottom_a = object1.y + object1.h;

    int left_b = object2.x;
    int right_b = object2.x + object2.w;
    int top_b = object2.y;
    int bottom_b = object2.y + object2.h;

    // case 1: size object 1 < size object 2
    if(left_a > left_b && left_a < right_b){
        if(top_a > top_b && top_a < bottom_b){
            return true;
        }
        if(bottom_a > top_b && bottom_a < bottom_b){
            return true;
        }
    }
    if(right_a > left_b && right_a < right_b){
        if(top_a > top_b && top_a < bottom_b){
            return true;
        }
        if(bottom_a > top_b && bottom_a < bottom_b){
            return true;
        }
    }

    // case 2: size object 1 > size object 2
    if(left_b > left_a && left_b < right_a){
        if(top_b > top_a && top_b < bottom_a){
            return true;
        }
        if(bottom_b > top_a && bottom_b < bottom_a){
            return true;
        }
    }
    if(right_b > left_a && right_b < right_a){
        if(top_b > top_a && top_b < bottom_a){
            return true;
        }
        if(bottom_b > top_a && bottom_b < bottom_a){
            return true;
        }
    }

    //case 3: size object 1 = size object 2
    if(top_a == top_b && bottom_a == bottom_b && right_a == right_b){
        return true;
    }
    return false;
}