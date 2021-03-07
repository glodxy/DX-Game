//
// Created by 田淙宇 on 2019/12/11.
//

#include "CameraController.h"
#include "../GameObject/Camera/Camera.h"
#include "Transform.h"
#include "Game/CharacterController.h"

using namespace GRCore;

CameraController::CameraController(float distance, GRCore::GameObject *obj) {
    mDistance=distance;
    mTarget=obj;
}

void CameraController::update() {
    XMFLOAT3 targetPos=mTarget->GetComponent<Transform>()->GetPosition();

    Camera* camera=(Camera*)mObject;
//
//    XMFLOAT3 targetPos=mTarget->GetComponent<Transform>()->GetPosition();
//
//    XMFLOAT3 resultPos=Add(mOffset,targetPos);
//    camera->SetPosition(resultPos.x,resultPos.y,resultPos.z);
//
//    XMVECTOR view=XMVector3Normalize(XMLoadFloat3(&mOffset));
//    XMFLOAT3 viewDir;
//    XMStoreFloat3(&viewDir,view);
//    camera->SetDirection(camera->GetUpDirection(),XMFLOAT3(-viewDir.x,-viewDir.y,-viewDir.z));

    float x=GR_MOUSE->GetPos().x;
    float y=GR_MOUSE->GetPos().y;
    if(mPressed) {
        float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMouseX));
        float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMouseY));

        camera->RotateX(dy);
        camera->RotateY(dx);
    }
    XMFLOAT3 resultPos=Sub(targetPos,Mul(camera->GetLookAtDirection(),mDistance));
    camera->SetPosition(resultPos.x,resultPos.y,resultPos.z);
    XMFLOAT3 lookatDirectionXZ=XMFLOAT3(camera->GetLookAtDirection().x,0,camera->GetLookAtDirection().z);
    XMVECTOR l=XMVector3Normalize(XMLoadFloat3(&lookatDirectionXZ));
    std::cout<<"lookat direction:"<<l<<std::endl;
    XMStoreFloat3(&lookatDirectionXZ,l);

    XMFLOAT3 right=XMFLOAT3(camera->GetRightDirection().x,0,camera->GetRightDirection().z);
    XMVECTOR r=XMVector3Normalize(XMLoadFloat3(&right));
    std::cout<<"right direction:"<<r<<std::endl;
    XMStoreFloat3(&right,r);

    mTarget->GetComponent<CharacterController>()->Set(lookatDirectionXZ,right);
//    XMFLOAT3 face=camera->GetLookAtDirection();
//    XMVECTOR f=XMVector3Normalize(XMLoadFloat3(&XMFLOAT3(face.x,.0f,face.z)));
//    XMStoreFloat3(&face,f);
//
//    CharacterController* controller=mTarget->GetComponent<CharacterController>();
//    controller->SetRight(camera->GetRightDirection());
//    controller->SetDirection(face);


    mLastMouseX=x;
    mLastMouseY=y;
    if(GR_MOUSE->IsLeftPressed()){
        mPressed=true;
    }else{
        mPressed=false;
    }
}