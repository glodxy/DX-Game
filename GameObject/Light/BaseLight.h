//
// Created by 田淙宇 on 2019/12/8.
//

#ifndef GAME_BASELIGHT_H
#define GAME_BASELIGHT_H

#include "../GameObject.h"

namespace GRCore {
    class BaseLight:public GameObject{
    public:
        BaseLight();

        void SetDiffuse(float r,float g,float b);

        LIGHT GetValue()const;

        virtual int GetLightType()const=0;
    protected:
        LIGHT mLight;
    };
}


#endif //GAME_BASELIGHT_H
