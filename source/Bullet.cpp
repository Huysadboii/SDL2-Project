#include "../header/Bullet.h"

Bullet::Bullet(){
    x_val_ = 0;
    y_val_ = 0;
    is_move_ = false;
    bullet_type_ = NORMAL_BULLET;
}

Bullet::~Bullet(){}

void Bullet::LoadImgBullet(SDL_Renderer* des){
    if(bullet_type_ == NORMAL_BULLET){
        LoadImg("img//leftBullet.png", des);
    } else if(bullet_type_ == ENEMY_BULLET){
        LoadImg("img//enemyBullet.png", des);
    } else if(bullet_type_ == ENEMY2_BULLET){
        LoadImg("img//enemy2Bullet.png", des);
    }
}

void Bullet::HandleMove(const int& x_border, const int& y_border){

    if(bullet_direction_ == DIR_RIGHT){
        rect_.x += x_val_;
        if(rect_.x > x_border || rect_.x < 0){
            is_move_ = false;
        }
    }

    else if(bullet_direction_ == DIR_LEFT){
        rect_.x -= x_val_;
        if(rect_.x > x_border || rect_.x < 0){
            is_move_ = false;
        }
    }
}