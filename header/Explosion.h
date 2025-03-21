#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "constant.h"
#include "Object.h"

class Explosion : public Object
{
    public:
        Explosion();
        ~Explosion();

        void set_clip();
        void set_frame(const int& fr) { frame = fr; }
        virtual bool LoadImg(string path, SDL_Renderer* screen);
        void Show(SDL_Renderer* des);
        int get_frame_width() const { return frame_width; }
        int get_frame_height() const { return frame_height; }

    private:
        int frame;
        int frame_width; // kich thuoc cua moi frame
        int frame_height;
        SDL_Rect frame_clip_[EXPLOSION_FRAME];
};

#endif