//
// Created by glodxy on 2019/12/27.
//

#include "Enemy.h"
#include "../../GameObject/GameObject.h"
#include "../Transform.h"
#include "../../GameObject/Game/Ball.h"
#include "../Animator.h"
#include "../../GameObject/Scene.h"
#include <ctime>
using namespace GRCore;

void Enemy::update() {
    if(mDead&&GR_TIMER->TotalTime()-mDeadTime>3.0f){
        DestroyObject(mObject->GetScene(),mObject);
    }
    Transform* trans=mObject->GetComponent<Transform>();
    XMFLOAT3 curPos=trans->GetPosition();
    if(GR_TIMER->TotalTime()-mStartTime>3.0f) {
        int i = rand() % 4;
        dir=i;
        mStartTime=GR_TIMER->TotalTime();
    }
    if(!mDead) {
        switch (dir) {
            case 0: {
                curPos = Add(curPos, Mul(XMFLOAT3(1.0f, .0f, .0f), mSpeed * GR_TIMER->DeltaTime()));
                trans->SetRotation(.0f, 0.5f * XM_PI, .0f);
                break;
            }
            case 1: {
                curPos = Add(curPos, Mul(XMFLOAT3(-1.0f, .0f, .0f), mSpeed * GR_TIMER->DeltaTime()));
                trans->SetRotation(.0f, -.5f * XM_PI, .0f);
                break;
            }
            case 2: {
                curPos = Add(curPos, Mul(XMFLOAT3(0.0f, .0f, 1.0f), mSpeed * GR_TIMER->DeltaTime()));
                trans->SetRotation(.0f, .01f, .0f);
                break;
            }
            case 3: {
                curPos = Add(curPos, Mul(XMFLOAT3(0.0f, .0f, -1.0f), mSpeed * GR_TIMER->DeltaTime()));
                trans->SetRotation(.0f, XM_PI, .0f);
                break;
            }
        }
        trans->SetPosition(curPos.x, curPos.y, curPos.z);
    }
}

void Enemy::OnCollision(std::vector<GameObject *> collisions) {
    if(!mDead) {
        for (auto iter:collisions) {
            if(iter!=nullptr) {
                std::string name = iter->GetName();
                std::cout << "collision name" << name << std::endl;
                if (name.find("ball") < name.size()) {
                    Ball* ball= dynamic_cast<Ball*>(iter);
                    if(ball->ballType==mType) {
                        std::cout << "destroy obj test" << std::endl;

                        mDead = true;
                        Animator *animator = mObject->GetComponent<Animator>();
                        animator->SetCurState("die");
                        mDeadTime = GR_TIMER->TotalTime();
                        break;
                    }

                }
            }
        }
    }
}

void Enemy::OnAddOnObject(GRCore::GameObject *obj) {
    srand(time(0));
    ModelMesh* mesh=obj->GetComponent<ModelMesh>();
    if (mType==WATERBALL) {
        mesh->SetMaterialName("water");
    }else if (mType == FIREBALL) {
        mesh->SetMaterialName("fire");
    }
    else {
        mesh->SetMaterialName("green");
    }
    int i=rand()%10+5;
    mSpeed=(float)i/10;
    int j = rand() % 4;
    dir=j;
    mStartTime=.0;

}