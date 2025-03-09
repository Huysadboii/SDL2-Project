#ifndef BULLET_H_
#define BULLET_H_

#include "BaseObject.h"
#include "constant.h"

class Bullet : public BaseObject{

    public:
        Bullet();
        ~Bullet();

        enum BULLET_DIR{
            DIR_RIGHT = 20, // khong dat enum trung nhau
            DIR_LEFT = 21,
        };

        void set_x_val(const int& xVal){x_val_ = xVal;}
        void set_y_val(const int& yVal){y_val_ = yVal;}
        int get_x_val() const {return x_val_;}
        int get_y_val() const {return y_val_;}

        void set_is_move(const bool& isMove){is_move_ = isMove;}
        bool get_is_move() const {return is_move_;}

        void HandleMove(const int& x_border, const int& y_border);
        void set_bullet_direction(const int& val){bullet_direction_ = val;}
        int get_bullet_direction() const {return bullet_direction_;}

    private:
        int x_val_; // doi tuong di chuyen luon co bien nay
        int y_val_;
        bool is_move_;
        int bullet_direction_;

};

#endif