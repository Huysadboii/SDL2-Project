#include "Explosion.h"

Explosion::Explosion()
{
    frame = 0;
    frame_width = 0;
    frame_height = 0;
}

Explosion::~Explosion(){}

bool Explosion::LoadImg(string path, SDL_Renderer* screen)
{
    bool ret = Object::LoadImg(path, screen);
    if(ret == true)
    {
        frame_width = rect_.w/EXPLOSION_FRAME;
        frame_height = rect_.h;
    }
    return ret;
}

void Explosion::set_clip(){
    if(frame_width > 0 && frame_height > 0)
    {
        for(int i = 0; i < EXPLOSION_FRAME; i++)
        {
            frame_clip_[i].x = i*frame_width;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = frame_width;
            frame_clip_[i].h = frame_height;
        }
    }
}

void Explosion::Show(SDL_Renderer* des)
{
    SDL_Rect* current_clip = &frame_clip_[frame];
    SDL_Rect renderQuad = {rect_.x, rect_.y, frame_width, frame_height};
    if(current_clip != NULL)
    {
        renderQuad.w = current_clip->w;
        renderQuad.h = current_clip->h;
    }
    SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
}