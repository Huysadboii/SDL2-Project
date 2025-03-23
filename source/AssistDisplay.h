#ifndef ASSIST_DISPLAY_H
#define ASSIST_DISPLAY_H

#include "constant.h"

struct Geometry{
    public:
        Geometry(int left, int top, int width, int height){
            left_ = left;
            top_ = top;
            width_ = width;
            height_ = height;
        }

        int left_, top_;
        int width_, height_;
};

struct ColorDat{
        ColorDat(Uint8 red, Uint8 green, Uint8 blue){ red_ = red; green_ = green; blue_ = blue; }
    public:
        Uint8 red_, green_, blue_;
};

class AssistDisplay{
    public:
        static void RenderRectangle(const Geometry& geo, const ColorDat& color, SDL_Renderer* des);
        static void RenderOutline(const Geometry& geo, const ColorDat& color, SDL_Renderer* des);
};

#endif