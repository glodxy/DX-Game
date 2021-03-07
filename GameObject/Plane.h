//
// Created by 田淙宇 on 2019/12/9.
//

#ifndef GAME_PLANE_H
#define GAME_PLANE_H

#include "GameObject.h"

namespace GRCore {
    class Plane:public GameObject {
    public:
        Plane();
        virtual ~Plane()=default;
    protected:
    };
}


#endif //GAME_PLANE_H
