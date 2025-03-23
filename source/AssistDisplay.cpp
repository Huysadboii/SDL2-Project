#include "AssistDisplay.h"

void AssistDisplay::RenderRectangle(const Geometry& geo, const ColorDat& color, SDL_Renderer* des){
    SDL_Rect fillRect = {geo.left_, geo.top_, geo.width_, geo.height_};
    SDL_SetRenderDrawColor(des, color.red_, color.green_, color.blue_, RENDER_DRAW_COLOR);
    SDL_RenderFillRect(des, &fillRect);
}

void AssistDisplay::RenderOutline(const Geometry& geo, const ColorDat& color, SDL_Renderer* des){
    SDL_Rect outlineRect = {geo.left_, geo.top_, geo.width_, geo.height_};
    SDL_SetRenderDrawColor(des, color.red_, color.green_, color.blue_, RENDER_DRAW_COLOR);
    SDL_RenderDrawRect(des, &outlineRect);
}