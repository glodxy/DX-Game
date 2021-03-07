//
// Created by glodxy on 2019/12/24.
//

#include "Animator.h"
#include "../GameObject/GameObject.h"
#include "../App/AnimManager.h"
#include "ModelMesh.h"
using namespace GRCore;

Animator::Animator() {
    mCurFrame=0;
    mIntervalFrame=1;
}

void Animator::AddAnimation(std::string stateName, std::string animClipName, float speed, bool infinite) {
    if(mAnimStates.find(stateName)!=mAnimStates.end()){
        std::cerr<<"animator of "<<mObject->GetName()<<" error add animation:"<<animClipName<<" to state:"<<stateName<<std::endl;
    }
    Animation anim;
    anim.animName=animClipName;
    anim.speed=speed;
    anim.infinite=infinite;
    mAnimStates[stateName]=anim;
}

void Animator::update() {
    if(mCurFrame==mIntervalFrame){
        Animation anim=mAnimStates[mCurState];
        ModelMesh* mm=mObject->GetComponent<ModelMesh>();
        AnimManager::GetAnimManager()->UpdateAnim(mm,anim.animName,anim.speed,anim.infinite);
    }
    mCurFrame=(mCurFrame+1)%(mIntervalFrame+1);
}

void Animator::SetInterval(int frames) {
    mIntervalFrame=frames;
}

void Animator::SetCurState(std::string state) {
    if(mAnimStates.find(state)==mAnimStates.end()){
        std::cerr<<mObject->GetName()<<"set anim state error"<<std::endl;
    }
    mCurState=state;
    AnimManager::GetAnimManager()->ResetAnimCount();
}