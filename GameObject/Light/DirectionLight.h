//
// Created by 田淙宇 on 2019/12/5.
//

#ifndef GAME_DIRECTIONLIGHT_H
#define GAME_DIRECTIONLIGHT_H

#include "BaseLight.h"

namespace GRCore {
    class DirectionLight:public BaseLight{
    public:
        DirectionLight();

        void SetDirection(float x,float y,float z);

        int GetLightType()const{
            return DIR_LIGHT_TYPE;
        }
    protected:

    };
}


#endif //GAME_DIRECTIONLIGHT_H
