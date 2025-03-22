#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "constant.h"
#include "Object.h"

class TiteMat : public Object{
    public:
        TiteMat(){;}
        ~TiteMat(){;}
};

//fill vi tri cac o vuong cho map
class GameMap{
    public:
        GameMap(){;}
        ~GameMap(){;}

        void LoadMap(char* name);
        void LoadTiles(SDL_Renderer* screen);
        void DrawMap(SDL_Renderer* screen);
        Map getMap() {return game_map_;}
        void SetMap(Map& map_data){game_map_ = map_data;};

    private:
        Map game_map_; // thong tin ve ban do
        TiteMat tile_mat[MAX_TILES]; // mang luu tru cac o vuong 

};

namespace Collision{
    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
}

#endif