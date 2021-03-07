//
// Created by 田淙宇 on 2019/12/8.
//

#ifndef GAME_SPOTLIGHT_H
#define GAME_SPOTLIGHT_H

#include "BaseLight.h"

namespace GRCore {
    class SpotLight:public BaseLight{
    public:
        SpotLight();

        void SetPosition(float x,float y,float z);
        void SetFalloff(float start,float end);
        void SetSpot(float s);
        void SetDirection(float x,float y,float z);

        int GetLightType()const{
            return SPOT_LIGHT_TYPE;
        }
    protected:
    };
}


#endif //GAME_SPOTLIGHT_H
