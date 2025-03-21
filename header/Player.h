#ifndef PLAYER_H_
#define PLAYER_H_
using namespace std;

#include "constant.h"
#include "Object.h"
#include "Bullet.h"

class Player : public Object{
    public:
        Player();
        ~Player();
        enum WalkType{
            WALK_RIGHT = 0,
            WALK_LEFT = 1,
        };
        
        // kich thuoc player thong so rieng => khai bao lai
        bool LoadImg(string path, SDL_Renderer* screen); 
        void Show(SDL_Renderer* des);
        void Handle_Input_Action(SDL_Event events, SDL_Renderer* screen);
        void set_clips();
        SDL_Rect GetRectFrame();
        int get_width_frame() const {return width_frame_;}
        int get_height_frame() const {return height_frame_;}
        
        void DoPlayer(Map& map_data);
        void CheckToMap(Map& map_data);
        void SetMapXY(const int map_x, const int map_y){map_x_ = map_x; map_y_ = map_y;};
        void CenterEntityOnMap(Map& map_data);

        void set_bullet_list(vector<Bullet*> bullet_list){p_bullet_list_ = bullet_list;}
        vector<Bullet*> get_bullet_list() const {return p_bullet_list_;}
        void HandleBullet(SDL_Renderer* des);
        void RemoveBullet(const int& idx);
        void IncreaseCoin();

        void set_comeback_time(const int& cb_time){come_back_time_ = cb_time;}

    private:
        vector<Bullet*> p_bullet_list_; // bang dan cua player
        int coin_count;

        float x_val_; // vi tri di chuyen theo phuong x
        float y_val_;
        
        float x_pos_; // vi tri hien tai cua player
        float y_pos_;
        
        int width_frame_;
        int height_frame_;

        SDL_Rect frame_clip_[PLAYER_FRAME]; // 8 frame cua player
        Input input_type_; // bien luu trang thai player
        int frame_;
        int status_; // trang thai cua player
        bool on_ground_;

        // luu vi tri ban do
        int map_x_;
        int map_y_;

        int come_back_time_; // thoi gian hoi sinh
};

#endif