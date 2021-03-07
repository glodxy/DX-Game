//
// Created by 田淙宇 on 2019/12/5.
//

#include "Transform.h"
#include "Collider/Collider.h"
#include "../GameObject/GameObject.h"
using namespace GRCore;

Transform::Transform() {
    mX=mY=mZ=0;
    mRX=mRY=mRZ=0;
    mSX=mSY=mSZ=1.f;
}

Transform::Transform(float *pos, float *rotation, float *scale) {
    mX=pos[0];
    mY=pos[1];
    mZ=pos[2];
    mRX=rotation[0];
    mRY=rotation[1];
    mRZ=rotation[2];
    mSX=scale[0];
    mSY=scale[1];
    mSZ=scale[2];
}

void Transform::SetPosition(float x, float y, float z) {
    mX=x;
    mY=y;
    mZ=z;
    Collider* collider=nullptr;
    if((collider=mObject->GetComponent<SphereCollider>())!=nullptr||(collider=mObject->GetComponent<RectCollider>())!=nullptr){
        std::cout<<"change collider pos"<<std::endl;
        collider->setPosition(x,y,z);
        collider->SetBoundCenter(XMFLOAT3(x,y,z));
        std::cout<<"set collider position over"<<std::endl;
    }
}

void Transform::SetRotation(float x, float y, float z) {
    mRX=x;
    mRY=y;
    mRZ=z;
}

void Transform::SetScale(float x, float y, float z) {
    mSX=x;
    mSY=y;
    mSZ=z;
}

XMFLOAT4X4 Transform::GetMatrix() const {
    XMMATRIX result=XMMatrixIdentity();
    result=result*XMMatrixScaling(mSX,mSY,mSZ);
    result=result*XMMatrixRotationX(mRX)*XMMatrixRotationY(mRY)*XMMatrixRotationZ(mRZ);
    result=result*XMMatrixTranslation(mX,mY,mZ);
    XMFLOAT4X4 matrix=XMFLOAT4X4();
    XMStoreFloat4x4(&matrix,result);
    return matrix;
}