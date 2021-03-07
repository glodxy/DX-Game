//
// Created by 田淙宇 on 2019/12/5.
//

#include "DirectionLight.h"
using namespace GRCore;

DirectionLight::DirectionLight():BaseLight()
{

}

void DirectionLight::SetDirection(float x, float y, float z) {
    mLight.direction=XMFLOAT3(x,y,z);
    XMVECTOR dir=XMLoadFloat3(&(mLight.direction));
    dir=XMVector3Normalize(dir);
    XMStoreFloat3(&(mLight.direction),dir);
}
