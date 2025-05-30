#ifndef ENEMY_H_
#define ENEMY_H_

#include "Object.h"
#include "constant.h"
#include "Bullet.h"

class Enemy : public Object{
    public:
        Enemy(); ~Enemy();
        void set_x_val(const double& xVal){ x_val_ = xVal; }
        void set_y_val(const double& yVal){ y_val_ = yVal; }
        void set_x_pos(const double& xPos){ x_pos_ = xPos; }
        void set_y_pos(const double& yPos){ y_pos_ = yPos; }
        double get_x_pos() const { return x_pos_; }
        double get_y_pos() const { return y_pos_; }
        int get_width_frame() const { return width_frame_; }
        int get_height_frame() const { return height_frame_; }
        SDL_Rect GetRectFrame();
        void SetMapXY(const int& map_x, const int& map_y){ map_x_ = map_x; map_y_ = map_y; }

        void set_clips();
        bool LoadImg(string path, SDL_Renderer* screen);
        void Show(SDL_Renderer* des);
        void DoEnemy(Map& g_map);
        void CheckToMap(Map& g_map);

        enum TypeMove{
            STATIC = 0,
            MOVE_IN_SPACE = 1,
            MOVE_ON_GROUND = 2
        };
        void set_type_move(const int& type_move){ type_move_ = type_move; }
        void set_animation_pos(const int& pos_a, const int& pos_b)
            { animation_a_ = pos_a; animation_b_ = pos_b; }
        void set_input_left(const int& input_left){ input_type_.left_ = input_left; }
        void ImpMoveType(SDL_Renderer* screen);

        vector<Bullet*> get_bullet_list() const { return bullet_list_; }
        void set_bullet_list(const vector<Bullet*>& bullet_list){ bullet_list_ = bullet_list; }
        void InitBullet(Bullet* p_bullet, SDL_Renderer* screen, int enemy_type);
        void MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit, Map& map_data);
        void RemoveBullet(const int& idx);

    private:
        int frame_, width_frame_, height_frame_;
        SDL_Rect frame_clip_[ENEMY_FRAME];

        bool on_ground_;
        int come_back_time_;
        double x_pos_, y_pos_; // current position
        double x_val_, y_val_; // change in position
        int map_x_, map_y_; // check limit of map

        int type_move_;
        int animation_a_, animation_b_;
        Input input_type_;

        vector<Bullet*> bullet_list_;
};

#endif