#include "../header/Enemy.h"
// xem lai
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

SDL_Rect Enemy::GetRectFrame(){
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;
    return rect;
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

void Enemy::RemoveBullet(const int& idx){
    int size = bullet_list_.size();
    if(size > 0 && idx < size){
        Bullet* p_bullet = bullet_list_.at(idx);
        bullet_list_.erase(bullet_list_.begin() + idx);
        if(p_bullet){
            delete p_bullet;
            p_bullet = NULL;
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
                LoadImg("img//threat_left.png", screen);
            }

        } else{
            if(input_type_.left_ == 1){
                LoadImg("img//threat_left.png", screen);
            } 
        }
    }
}
// xem lai
void Enemy::InitBullet(Bullet* p_bullet, SDL_Renderer* screen, int enemy_type){
    if(p_bullet != NULL){
        if(enemy_type == 1){
            p_bullet->set_bullet_type(Bullet::ENEMY_BULLET);
        } else if(enemy_type == 2){
            p_bullet->set_bullet_type(Bullet::ENEMY2_BULLET);
        }
        p_bullet->LoadImgBullet(screen);
        p_bullet->set_is_move(true);
        p_bullet->set_bullet_direction(Bullet::DIR_LEFT);
        p_bullet->SetRect(rect_.x + 5, rect_.y + 10);
        p_bullet->set_x_val(15);
        bullet_list_.push_back(p_bullet);
    }
}

void Enemy::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit){
    for(int i = 0; i < bullet_list_.size(); i++){

        Bullet* p_bullet = bullet_list_.at(i);
        if(p_bullet != NULL){

            if(p_bullet->get_is_move()){
                int bullet_distance = rect_.x + width_frame_ - p_bullet->GetRect().x;
                if(bullet_distance < MAX_BULLET_DIS && bullet_distance > 0){
                    p_bullet->HandleMove(x_limit, y_limit);
                    p_bullet->Render(screen);
                } else {
                    p_bullet->set_is_move(false);
                }

            } else{
                p_bullet->set_is_move(true);
                p_bullet->SetRect(rect_.x + 5, rect_.y + 10);
            }
            
        }

    }
}