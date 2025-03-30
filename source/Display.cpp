#include "Display.h"

Display::Display(){
    num_ = 0;
}

Display::~Display(){}

void Display::AddPos(const int& pos){
    pos_list_.push_back(pos);
}

void Display::Init(SDL_Renderer* screen){
    bool ret = LoadImg("img//player_life.png", screen);
    num_ = LIFE;
    if(pos_list_.size() > 0){
        pos_list_.clear();
    }

    for(int i=0; i<LIFE; i++) AddPos(20+40*i);
}

void Display::Show(SDL_Renderer* des){
    for(int i=0; i<pos_list_.size(); i++){
        rect_.x = pos_list_.at(i);
        rect_.y = 0;
        Render(des);
    }
}

void Display::Decrease(){
    num_--;
    pos_list_.pop_back();
}

void Display::Increase(){
    num_++;
    AddPos(20 + 40*(num_-1));
}

DisplayCoin::DisplayCoin(){
    x_pos_ = 0;
    y_pos_ = 0;
}

DisplayCoin::~DisplayCoin(){}

void DisplayCoin::Init(SDL_Renderer* screen){
    bool ret = LoadImg("img//money.png", screen);
}

void DisplayCoin::Show(SDL_Renderer* des){
    rect_.x = x_pos_;
    rect_.y = y_pos_;
    Render(des);
}