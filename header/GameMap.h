#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "constant.h"
#include "BaseObject.h"

#define MAX_TILES 20 // so loai o vuong

// map cung la vat the nen ke thua BaseObject
class TiteMat : public BaseObject{
    public:
    TiteMat(){;}
    ~TiteMat(){;}
};

//fill vi tri cac o vuong cho map
class GameMap{
    public:
    GameMap(){;}
    ~GameMap(){;}

    // doc thong tin tu file so do map
    void LoadMap(char* name);
    // load hinh anh cho tilemat
    void LoadTiles(SDL_Renderer* screen);
    // ve map
    void DrawMap(SDL_Renderer* screen);

    private:
    Map game_map_; // thong tin ve ban do
    TiteMat tile_mat[MAX_TILES]; // mang luu tru cac o vuong 

};

#endif