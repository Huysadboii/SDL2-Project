#include "../header/Player.h"

Player::Player(){
    x_val_ = 0;
    y_val_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    frame_ = 0;
    status_ = -1;
    input_type_.left_ = 0;
    input_type_.right_ = 0;
    input_type_.jump_ = 0;
    input_type_.down_ = 0;
    input_type_.up_ = 0;
    on_ground_ = false;
    map_x_ = 0;
    map_y_ = 0;
    come_back_time_ = 0;
}

Player::~Player(){
}

bool Player::LoadImg(string path, SDL_Renderer* screen){
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret){
        width_frame_ = rect_.w/8;
        height_frame_ = rect_.h;
    }
    return ret;
}

void Player::set_clips(){
    if(width_frame_>0 && height_frame_>0){
        for(int i=0; i<8; i++){
            frame_clip_[i].x = i*width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

void Player::Show(SDL_Renderer* des){

    if(status_ == WALK_LEFT){
        LoadImg("img//player_left.png", des);
    } else {
        LoadImg("img//player_right.png", des);
    }
    
    if(input_type_.left_ == 1 || input_type_.right_ == 1){
        frame_++;
    } else {
        frame_ = 0;
    }
    
    if(frame_ >= 8){
        frame_ = 0;
    }
    
    if(come_back_time_ == 0){ // do ton tai nguyen hoi sinh
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;
        
        SDL_Rect* current_clip = &frame_clip_[frame_];
        SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
        SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
    }
    
}

void Player::Handle_Input_Action(SDL_Event events, SDL_Renderer* screen){
    
    if(events.type == SDL_KEYDOWN){

        switch(events.key.keysym.sym){
            case SDLK_d:
                status_ = WALK_RIGHT;
                input_type_.right_ = 1;
                input_type_.left_ = 0;
                break;
            case SDLK_a:
                status_ = WALK_LEFT;
                input_type_.left_ = 1;
                input_type_.right_ = 0;
                break;
            case SDLK_SPACE:
                input_type_.jump_ = 1;
                break;
        }

    } 
    
    else if(events.type == SDL_KEYUP){

        switch(events.key.keysym.sym){
            case SDLK_d:
                input_type_.right_ = 0;
                break;
            case SDLK_a:
                input_type_.left_ = 0;
                break;
        }

    }

    else if(events.type == SDL_MOUSEBUTTONDOWN){

        if(events.button.button == SDL_BUTTON_LEFT){

            Bullet* p_bullet = new Bullet();

            if(status_ == WALK_LEFT){
                p_bullet->LoadImg("img//leftBullet.png", screen);
                p_bullet->set_bullet_direction(Bullet::DIR_LEFT);
                p_bullet->SetRect(this->rect_.x, 
                    this->rect_.y + height_frame_*DECLINE_BULLET);
            } else {
                p_bullet->LoadImg("img//rightBullet.png", screen);
                p_bullet->set_bullet_direction(Bullet::DIR_RIGHT);
                p_bullet->SetRect(this->rect_.x + width_frame_ - 20, 
                    this->rect_.y + height_frame_*DECLINE_BULLET);
            }

            p_bullet->set_x_val(2*PLAYER_SPEED);
            p_bullet->set_is_move(true);
            p_bullet_list_.push_back(p_bullet);

        }

    }

}

void Player::HandleBullet(SDL_Renderer* des){

    for(int i=0; i<p_bullet_list_.size(); i++){

        Bullet* p_bullet = p_bullet_list_.at(i);

        if(p_bullet != NULL){

            if(p_bullet->get_is_move()){
                p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                p_bullet->Render(des);
            } else {
                p_bullet_list_.erase(p_bullet_list_.begin() + i);
                if(p_bullet != NULL){
                    delete p_bullet;
                    p_bullet = NULL;
                }
            }

        }

    }

}

void Player::DoPlayer(Map& map_data){

    if(come_back_time_ == 0){
        x_val_ = 0;
        y_val_ += GRAVITY;

        if(y_val_ >= MAX_FALL_SPEED){
            y_val_ = MAX_FALL_SPEED;
        }

        if(input_type_.left_ == 1){
            x_val_ -= PLAYER_SPEED;
        } else if(input_type_.right_ == 1){
            x_val_ += PLAYER_SPEED;
        }

        if(input_type_.jump_ == 1){
            if(on_ground_ == true){
                y_val_ = -PLAYER_JUMP_VAL;
                on_ground_ = false;
            }
            input_type_.jump_ = 0;
        }

        CheckToMap(map_data);
        CenterEntityOnMap(map_data);
    }

    else if(come_back_time_ > 0){
        come_back_time_--;

        if(come_back_time_ == 0){

            if(x_pos_ > 9*TILE_SIZE){
                x_pos_ -= 9*TILE_SIZE;
                map_x_ -= 9*TILE_SIZE;
            } else {
                x_pos_ = 0;
            }

            y_pos_ = 0;
            x_val_ = 0;
            y_val_ = 0;

        }

    }

}

void Player::CenterEntityOnMap(Map& map_data){
    map_data.start_x_ = x_pos_ - (SCREEN_WIDTH/2);
    if(map_data.start_x_ < 0){ // di lui
        map_data.start_x_ = 0;
    } else if(map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_){
        map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
    }

    map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT/2);
    if(map_data.start_y_ < 0){
        map_data.start_y_ = 0;
    } else if(map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_){
        map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
    }
}

void Player::CheckToMap(Map& map_data){
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    // check horizontal
    int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;
    x1 = (x_pos_ + x_val_)/TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1)/TILE_SIZE;
    y1 = (y_pos_)/TILE_SIZE;
    y2 = (y_pos_ + height_min - 1)/TILE_SIZE;
    // check player co nam trong ban do hay khong
    if(x1>=0 && x2<MAX_MAP_X && y1>=0 && y2<MAX_MAP_Y){

        if(x_val_ > 0){ // player moves right
            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];

            if(val1 != BLANK_TILE || val2 != BLANK_TILE){
                x_pos_ = x2*TILE_SIZE; // khong vuot qua bien cua map
                x_pos_ -= width_frame_ + 1;
                x_val_ = 0;
            } 

        } else if(x_val_ < 0){
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];

            if(val1 != BLANK_TILE || val2 != BLANK_TILE){
                x_pos_ = (x1+1)*TILE_SIZE;
                x_val_ = 0;
            } 
        }

    }

    // check vertical
    int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
    x1 = (x_pos_)/TILE_SIZE;
    x2 = (x_pos_ + width_min )/TILE_SIZE;
    y1 = (y_pos_ + y_val_)/TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_ - 1)/TILE_SIZE;

    if(x1>=0 && x2<MAX_MAP_X && y1>=0 && y2<MAX_MAP_Y){

        if(y_val_ > 0){ // player moves down
            int val1 = map_data.tile[y2][x1];
            int val2 = map_data.tile[y2][x2];

            if(val1 != BLANK_TILE || val2 != BLANK_TILE){
                y_pos_ = y2*TILE_SIZE;
                y_pos_ -= height_frame_ + 1;
                y_val_ = 0;
                on_ground_ = true;
            }

        } else if(y_val_ < 0){ // player moves up => decline
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];

            if(val1 != BLANK_TILE || val2 != BLANK_TILE){
                y_pos_ = (y1+1)*TILE_SIZE;
                y_val_ = 0;
            }
        }
        
    }

    x_pos_ += x_val_;
    y_pos_ += y_val_;

    if(x_pos_ < 0){
        x_pos_ = 0;
    } else if(x_pos_ + width_frame_ > map_data.max_x_){
        x_pos_ = map_data.max_x_ - width_frame_ - 1;
    }

    if(y_pos_ > map_data.max_y_){
        come_back_time_ = COMEBACK_TIME;
    }
}