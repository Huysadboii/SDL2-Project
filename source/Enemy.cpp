#include "../header/Enemy.h"

Enemy::Enemy(){
    x_val_ = 0; y_val_ = 0;
    x_pos_ = 0; y_pos_ = 0;
    width_frame_ = 0; height_frame_ = 0; frame_ = 0;
    on_ground_ = false;
    come_back_time_ = 0;

    animation_a_ = 0; animation_b_ = 0;
    input_type_.left_ = 0;
    type_move_ = STATIC;
}

Enemy::~Enemy(){}

bool Enemy::LoadImg(string path, SDL_Renderer* screen){
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret){ // if load image successfully
        width_frame_ = rect_.w / ENEMY_FRAME;
        height_frame_ = rect_.h;
    }
    return ret;
}

void Enemy::set_clips(){
    if(width_frame_ > 0 && height_frame_ > 0){
        for(int i = 0; i < ENEMY_FRAME; i++){
            frame_clip_[i].x = i * width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

void Enemy::Show(SDL_Renderer* des){
    if(come_back_time_ == 0){
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;
        frame_++; // thay doi lien tuc frame
        if(frame_ >= ENEMY_FRAME){
            frame_ = 0;
        }
        SDL_Rect* current_clip = &frame_clip_[frame_];
        SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
        SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
    }
}

void Enemy::DoEnemy(Map& g_map){
    if(come_back_time_ == 0){
        x_val_ = 0;
        y_val_ += GRAVITY;
        if(y_val_ >= MAX_FALL_SPEED){
            y_val_ = MAX_FALL_SPEED;
        }
        
        if(input_type_.left_ == 1){
            x_val_ -= ENEMY_SPEED;
        } else if(input_type_.right_ == 1){
            x_val_ += ENEMY_SPEED;
        }
        CheckToMap(g_map);
    }

    else if(come_back_time_ > 0){
        come_back_time_--;
        if(come_back_time_ == 0){
            x_val_ = 0;
            y_val_ = 0;

            if(x_pos_ > 256){
                x_pos_ -= 256;
                animation_a_ -= 256;
                animation_b_ -= 256;
            } else{
                x_pos_ = 0;
            }
            y_pos_ = 0;
            input_type_.left_ = 1;
        }
    }
}

void Enemy::CheckToMap(Map& map_data){
    int x1 = 0, x2 = 0;
    int y1 = 0, y2 = 0;

    // check horizontal
    int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;
    x1 = (x_pos_ + x_val_) / TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

    y1 = (y_pos_) / TILE_SIZE;
    y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y){
        if(x_val_ > 0){ // move right
            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];

            if( (val1 != BLANK_TILE && val1 != COIN_TILE) 
            ||  (val2 != BLANK_TILE && val2 != COIN_TILE) ){
                x_pos_ = x2 * TILE_SIZE;
                x_pos_ -= (width_frame_ + 1);
                x_val_ = 0;
            }

        } else if(x_val_ < 0){ // move left
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];

            if( (val1 != BLANK_TILE && val1 != COIN_TILE) 
            ||  (val2 != BLANK_TILE && val2 != COIN_TILE) ){
                x_pos_ = (x1 + 1) * TILE_SIZE;
                x_val_ = 0;
            }
        }
    }

    // check vertical
    int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
    x1 = (x_pos_) / TILE_SIZE;
    x2 = (x_pos_ + width_min) / TILE_SIZE;

    y1 = (y_pos_ + y_val_) / TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_ - 1) / TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y){
        if(y_val_ > 0){ // move down
            int val1 = map_data.tile[y2][x1];
            int val2 = map_data.tile[y2][x2];

            if( (val1 != BLANK_TILE && val1 != COIN_TILE)
            ||  (val2 != BLANK_TILE && val2 != COIN_TILE) ){
                y_pos_ = y2 * TILE_SIZE;
                y_pos_ -= (height_frame_ + 1);
                y_val_ = 0;
                on_ground_ = true;
            }
        }

        else if(y_val_ < 0){ // move up
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];

            if( (val1 != BLANK_TILE && val1 != COIN_TILE)
            ||  (val2 != BLANK_TILE && val2 != COIN_TILE) ){
                y_pos_ = (y1 + 1) * TILE_SIZE;
                y_val_ = 0;
            }
        }
    }

    x_pos_ += x_val_;
    y_pos_ += y_val_;
    if(x_pos_ < 0){
        x_pos_ = 0;
    }
    else if(x_pos_ + width_frame_ > map_data.max_x_){
        x_pos_ = map_data.max_x_ - width_frame_ - 1;
    }

    if(y_pos_ > map_data.max_y_){
        come_back_time_ = COMEBACK_TIME;
    }
}

void Enemy::ImpMoveType(SDL_Renderer* screen){
    if(type_move_ == STATIC) {;}
    else{

        if(on_ground_ == true){
            if(x_pos_ > animation_b_){
                input_type_.left_ = 1;
                input_type_.right_ = 0;
                LoadImg("img//threat_left.png", screen);
            } else if(x_pos_ < animation_a_){
                input_type_.left_ = 0;
                input_type_.right_ = 1;
                LoadImg("img//threat_right.png", screen);
            }

        } else{
            if(input_type_.left_ == 1){
                LoadImg("img//threat_left.png", screen);
            } 
        }
    }
}