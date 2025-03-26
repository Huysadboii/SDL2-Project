#ifndef PLAYER_H_
#define PLAYER_H_

#include "constant.h"
#include "Object.h"
#include "Bullet.h"

class Player : public Object{
    public:
        Player(); ~Player();
        enum WalkType{
            WALK_RIGHT = 0,
            WALK_LEFT = 1,
        };
        
        // kich thuoc player thong so rieng => khai bao lai
        bool LoadImg(string path, SDL_Renderer* screen); 
        void Show(SDL_Renderer* des);
        void Handle_Input_Action(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* bullet_sound[0]);
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
        void HandleBullet(SDL_Renderer* des, Map& map_data);
        void RemoveBullet(const int& idx);
        void IncreaseCoin();
        int get_coin_count() const {return coin_count;}

        void set_comeback_time(const int& cb_time){come_back_time_ = cb_time;}

    private:
        vector<Bullet*> p_bullet_list_;
        int coin_count;
        float x_val_, y_val_;
        float x_pos_, y_pos_;
        int width_frame_, height_frame_;

        SDL_Rect frame_clip_[PLAYER_FRAME];
        Input input_type_;
        int frame_;
        int status_;
        bool on_ground_;

        // luu vi tri ban do
        int map_x_, map_y_;

        int come_back_time_;
};

#endif