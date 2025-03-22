#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "constant.h"
#include "Object.h"

class Display : public Object{
    public:
        Display();
        ~Display();
        void SetNum(const int& num) { num_ = num; }
        void AddPos(const int& pos);
        void Show(SDL_Renderer* des);
        void Init(SDL_Renderer* screen);
        void Increase();
        void Decrease();

    private:
        int num_;
        vector<int> pos_list_;
};

class DisplayCoin : public Object{
    public:
        DisplayCoin();
        ~DisplayCoin();
        void Show(SDL_Renderer* des);
        void Init(SDL_Renderer* screen);
        void SetPos(const int& xPos, const int& yPos){ x_pos_ = xPos; y_pos_ = yPos; }

    private:
        int x_pos_, y_pos_;
};

#endif