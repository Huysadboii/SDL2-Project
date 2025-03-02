#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
using namespace std;

#define el "\n"

int main(int argc, char** argv){
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        cout<<"Working"<<el;
    }
    return 0;
}