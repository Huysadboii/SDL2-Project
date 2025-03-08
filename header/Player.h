#ifndef PLAYER_H_
#define PLAYER_H_
using namespace std;

#include "constant.h"
#include "BaseObject.h" // ban chat player la 1 object

class Player : public BaseObject{
    public:
        Player();
        ~Player();
        enum WalkType{
            WALK_RIGHT = 0,
            WALK_LEFT = 1
        };
        
        // kich thuoc player thong so rieng => khai bao lai
        bool LoadImg(string path, SDL_Renderer* screen); 
        void Show(SDL_Renderer* des);
        void Handle_Input_Action(SDL_Event events, SDL_Renderer* screen);
        void set_clips();

        

    private:
        float x_val_; // vi tri di chuyen theo phuong x
        float y_val_;

        float x_pos_; // vi tri hien tai cua player
        float y_pos_;
        
        int width_frame_;
        int height_frame_;

        SDL_Rect frame_clip_[8]; // 8 frame cua player
        Input input_type_; // bien luu trang thai player
        int frame_;
        int status_; // trang thai cua player: trai hay phai
};

#endif