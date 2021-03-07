//
// Created by glodxy on 2019/12/25.
//

#include "BallController.h"
#include "../../GameObject/Game/Ball.h"
#include "../Transform.h"

using namespace GRCore;

void BallController::update() {
    Ball* ball=(Ball*)mObject;
    GRCore::Transform* t=ball->GetComponent<Transform>();
    XMFLOAT3 curPos=t->GetPosition();
    XMFLOAT3 dir=ball->GetDirection();
    float speed=ball->GetSpeed();
    float off=GR_TIMER->DeltaTime()*speed;
    t->SetPosition(curPos.x+dir.x*off,curPos.y+dir.y*off,curPos.z+dir.z*off);
    if(GR_TIMER->TotalTime()-ball->mStartTime>1.0f){
        DestroyObject(mObject->GetScene(),mObject);
    }
}