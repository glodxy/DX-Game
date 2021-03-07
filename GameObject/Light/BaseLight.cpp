//
// Created by 田淙宇 on 2019/12/8.
//

#include "BaseLight.h"

using namespace GRCore;

BaseLight::BaseLight() {
    mLight.diffuse=XMFLOAT3(1.0f,1.f,1.f);
    mLight.position=XMFLOAT3(.0f,.0f,.0f);
    mLight.direction=XMFLOAT3(.0f,-1.f,.0f);
    mLight.falloffStart=1.0f;
    mLight.falloffEnd=2.0f;
    mLight.spot=1.0f;
}

void BaseLight::SetDiffuse(float r, float g, float b) {
    mLight.diffuse=XMFLOAT3(r,g,b);
}

LIGHT BaseLight::GetValue() const {
    return mLight;
}