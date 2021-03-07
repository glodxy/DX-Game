//
// Created by 田淙宇 on 2019/12/8.
//

#ifndef GAME_POINTLIGHT_H
#define GAME_POINTLIGHT_H

#include "BaseLight.h"

namespace GRCore {
    class PointLight:public BaseLight{
    public:
        PointLight();

        void SetPosition(float x,float y,float z);
        void SetFalloff(float start,float end);

        int GetLightType()const{
            return POINT_LIGHT_TYPE;
        }
    protected:

    };
}


#endif //GAME_POINTLIGHT_H
