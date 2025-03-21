#include "../header/Text.h"
// xem lai
Text::Text(){
    text_color_ = {255, 255, 255};
    text_val_ = NULL;
}

Text::~Text(){}

bool Text::LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen){
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, str_val_.c_str(), text_color_);
    if(text_surface){
        text_val_ = SDL_CreateTextureFromSurface(screen, text_surface);
        width_ = text_surface->w;
        height_ = text_surface->h;
        SDL_FreeSurface(text_surface);
    }
    return text_val_ != NULL;
}

void Text::Free(){
    if(text_val_ != NULL){
        SDL_DestroyTexture(text_val_);
        text_val_ = NULL;
    }
}
// xem lai
void Text::SetColor(Uint8 red, Uint8 green, Uint8 blue){
    text_color_ = {red, green, blue};
}

void Text::SetColor(int type){
    if(type == RED_TEXT){
        SDL_Color color = {255, 0, 0};
        text_color_ = color;
    } else if(type == WHITE_TEXT){
        SDL_Color color = {255, 255, 255};
        text_color_ = color;
    } else if(type == BLACK_TEXT){
        SDL_Color color = {0, 0, 0};
        text_color_ = color;
    }
}

void Text::RenderText(SDL_Renderer* screen, 
                      int xp, int yp, 
                      SDL_Rect* clip, 
                      double angle, 
                      SDL_Point* center, 
                      SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {xp, yp, width_, height_};
    if(clip != NULL){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(screen, text_val_, clip, &renderQuad, angle, center, flip);
}