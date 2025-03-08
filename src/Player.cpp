#include "../header/Player.h"
// luu y link path cua anh

Player::Player(){
    x_val_ = 0;
    y_val_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    frame_ = 0;
    status_ = -1;
}

Player::~Player(){
    Free();
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
    
    rect_.x = x_pos_;
    rect_.y = y_pos_;
    
    SDL_Rect* current_clip = &frame_clip_[frame_];
    SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
    SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
}

void Player::Handle_Input_Action(SDL_Event events, SDL_Renderer* screen){
    
    if(events.type == SDL_KEYDOWN){

        switch(events.key.keysym.sym){
            case SDLK_RIGHT:
                status_ = WALK_RIGHT;
                input_type_.right_ = 1;
                break;
            case SDLK_LEFT:
                status_ = WALK_LEFT;
                input_type_.left_ = 1;
                break;
        }

    } 
    
    else if(events.type == SDL_KEYUP){

        switch(events.key.keysym.sym){
            case SDLK_RIGHT:
                input_type_.right_ = 0;
                break;
            case SDLK_LEFT:
                input_type_.left_ = 0;
                break;
        }

    }
}