// create an image for basic object
#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_
using namespace std;

#include "constant.h"
class BaseObject
{
    public:
    BaseObject();
    ~BaseObject();

    void SetRect(const int& x, const int& y){ rect_.x = x; rect_.y = y; };
    SDL_Rect GetRect() { return rect_; }
    SDL_Texture* GetObject() { return p_object_; }
    bool LoadImg(string path, SDL_Renderer* screen);
    void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
    void Free();

    private:
    SDL_Texture* p_object_;
    SDL_Rect rect_; // size
};

#endif