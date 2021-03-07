//
// Created by 田淙宇 on 2019/12/9.
//

#ifndef GAME_TERRAIN_H
#define GAME_TERRAIN_H

#include "GameObject.h"

namespace GRCore {
    class Terrain:public  GameObject{
    public:
        Terrain();
        virtual ~Terrain()=default;
    protected:
    };
}


#endif //GAME_TERRAIN_H
