#ifndef OBJECT_H_
#define OBJECT_H_

#include "constant.h"

class Object
{
    public:
        Object(); ~Object();

        void SetRect(const int& x, const int& y){ rect_.x = x; rect_.y = y; };
        SDL_Rect GetRect() { return rect_; }
        SDL_Texture* GetObject() { return p_object_; }
        virtual bool LoadImg(string path, SDL_Renderer* screen); // để cho lớp con có thể ghi đè
        void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
        void Free();

    protected:
        SDL_Texture* p_object_;
        SDL_Rect rect_; // vi tri cua object
        // pos la vi tri tren ca map
};

#endif