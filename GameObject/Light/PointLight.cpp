//
// Created by 田淙宇 on 2019/12/8.
//

#include "PointLight.h"

using namespace GRCore;
PointLight::PointLight():BaseLight(){

}

void PointLight::SetPosition(float x, float y, float z) {
    mLight.position=XMFLOAT3(x,y,z);
}

void PointLight::SetFalloff(float start, float end) {
    mLight.falloffStart=start;
    mLight.falloffEnd=end;
}
