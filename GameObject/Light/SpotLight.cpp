//
// Created by 田淙宇 on 2019/12/8.
//

#include "SpotLight.h"

using namespace GRCore;

SpotLight::SpotLight() :BaseLight(){
}

void SpotLight::SetPosition(float x, float y, float z) {
    mLight.position=XMFLOAT3(x,y,z);
}

void SpotLight::SetFalloff(float start, float end) {
    mLight.falloffStart=start;
    mLight.falloffEnd=end;
}

void SpotLight::SetSpot(float s) {
    if(s<0){
        std::cerr<<"error set spot light spot:"<<s<<std::endl;
    }
    mLight.spot=s;
}

void SpotLight::SetDirection(float x, float y, float z) {
    mLight.direction=XMFLOAT3(x,y,z);
}
