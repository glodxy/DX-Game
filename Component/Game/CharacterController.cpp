//
// Created by glodxy on 2019/12/25.
//

#include "CharacterController.h"
#include "../../GameObject/GameObject.h"
#include "../Transform.h"
#include "../../GameObject/Game/Ball.h"
#include "../Animator.h"
#include "../../GameObject/Scene.h"
#include <math.h>
using namespace GRCore;

CharacterController::CharacterController(float speed) {
    mDirection=XMFLOAT3(.0f,.0f,1.0f);
    mRight=XMFLOAT3(1.0f,.0f,.0f);
    mSpeed=speed;
    mState=IDLE;
    mFront = { 0.0f,0.0f,1.0f };
    mRight = { 1.0f,0.0f,0.0f };
}
void CharacterController::Set(XMFLOAT3 front, XMFLOAT3 right) {
    mFront = front;
    mRight = right;
    GRCore::Transform* trans = mObject->GetComponent<Transform>();
    double cosAlpha = front.z / sqrt(front.x * front.x + front.y * front.y + front.z * front.z);
    double alpha = acos(cosAlpha);
    if(front.x>0){
        alpha=2*XM_PI-alpha;
    }
    trans->SetRotation(0,-alpha, 0);
}
void CharacterController::update() {
    GRCore::Transform* trans=mObject->GetComponent<Transform>();
    XMFLOAT2 mpos=GR_MOUSE->GetPos();
    XMFLOAT3 curPos=trans->GetPosition();

    float distance=mSpeed*GR_TIMER->DeltaTime();

//    mDirection.x=mpos.x-trans->GetPosition().x;
//    mDirection.z=mpos.y-trans->GetPosition().z;
//    XMVECTOR dir=XMVector3Normalize(XMLoadFloat3(&mDirection));
//    XMStoreFloat3(&mDirection,dir);
//
//    XMVECTOR up=XMLoadFloat3(&XMFLOAT3(.0f,1.0f,.0f));
//    XMVECTOR r=XMVector3Cross(up,dir);
//    XMStoreFloat3(&right,r);


    if(GR_KEYBOARD->IsKeyPressed(Keyboard::W)){
        if(mState!=RUN){
            mState=WALK;
            std::cout<<"state changed"<<std::endl;
            curPos=Add(curPos,Mul(mFront,distance));
            trans->SetPosition(curPos.x,curPos.y,curPos.z);
        }
    }else if(GR_KEYBOARD->IsKeyPressed(Keyboard::S)){
        if(mState!=RUN){
            mState=WALK;
            curPos=Sub(curPos,Mul(mFront,distance));
            trans->SetPosition(curPos.x,curPos.y,curPos.z);
        }
    }

    if(GR_KEYBOARD->IsKeyPressed(Keyboard::A)){
        if(mState!=RUN){
            mState=WALK;
            curPos=Sub(curPos,Mul(mRight,distance));
            trans->SetPosition(curPos.x,curPos.y,curPos.z);
        }
    }else if(GR_KEYBOARD->IsKeyPressed(Keyboard::D)){
        if(mState!=RUN){
            mState=WALK;
            curPos=Add(curPos,Mul(mRight,distance));
            trans->SetPosition(curPos.x,curPos.y,curPos.z);
        }
    }
    else if(GR_KEYBOARD->IsKeyPressed(Keyboard::Z)){
        mState = ATTACK;
        if(GR_TIMER->TotalTime()-currTime>0.5){
            currTime = GR_TIMER->TotalTime();
            Ball *ball = new Ball(Add(trans->GetPosition(),Mul(XMFLOAT3(.0f,1.0f,.0f),1.5f)),mFront,1,10.0f);
            count++;
            std::string name = "ball"+std::to_string(count);
            ball->SetName(name);
            mObject->GetScene()->AddChild(nullptr, ball);
            mObject->GetScene()->SetRenderType(ball, RT_Transparent);
        }
        
    }
    else if (GR_KEYBOARD->IsKeyPressed(Keyboard::X)) {
        mState = ATTACK;
        if (GR_TIMER->TotalTime() - currTime > 0.5) {
            currTime = GR_TIMER->TotalTime();
            Ball* ball = new Ball(Add(trans->GetPosition(),Mul(XMFLOAT3(.0f,1.0f,.0f),1.5f)), mFront, 2,10.0f);
            count++;
            std::string name = "ball" + std::to_string(count);
            ball->SetName(name);
            mObject->GetScene()->AddChild(nullptr, ball);
        }

    }
    else if (GR_KEYBOARD->IsKeyPressed(Keyboard::C)) {
        mState = ATTACK;
        if (GR_TIMER->TotalTime() - currTime > 0.5) {
            currTime = GR_TIMER->TotalTime();
            Ball* ball = new Ball(Add(trans->GetPosition(),Mul(XMFLOAT3(.0f,1.0f,.0f),1.5f)), mFront, 3,10.0f);
            count++;
            std::string name = "ball" + std::to_string(count);
            ball->SetName(name);
            mObject->GetScene()->AddChild(nullptr, ball);
        }

    }
    GRCore::Animator *anim = mObject->GetComponent<Animator>();
    std::string state;
    switch (mState) {
        case ATTACK:
        {
            state = "attack";
            break;
        }

        case WALK: {
            state = "walk";
            break;
        }
        case RUN: {
            state = "run";
            break;
        }
        case IDLE: {
            state = "idle";
            break;
        }
    }

    if(anim->GetCurState()!=state) {
        std::cout<<"change anim state"<<std::endl;
        anim->SetCurState(state);
    }

}