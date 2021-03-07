//
// Created by glodxy on 2019/12/24.
//

#include "AnimManager.h"

#include "../Component/ModelMesh.h"
#include "D3DSourceManager.h"


using namespace GRCore;
AnimManager* GRCore::AnimManager::mAnimManager=nullptr;

AnimManager* AnimManager::GetAnimManager() {
    if(mAnimManager==nullptr){
        mAnimManager=new AnimManager();
    }
    return mAnimManager;
}

AnimManager::AnimManager() {}

bool AnimManager::LoadAnimationClip(std::string clipName, std::string originName, float startTicks, float duration) {
    if(mAnimationMap.find(clipName)!=mAnimationMap.end()){
        std::cerr<<"error load animation clip:"<<clipName<<std::endl;
    }
    AnimationClip clip;
    clip.orginAnimName=originName;
    clip.startTicks=startTicks;
    clip.duration=duration;

    mAnimationMap[clipName]=clip;
}

void AnimManager::UpdateAnim(GRCore::ModelMesh *mm, const std::string &clipName,float speed,bool infinite) {
    if(mAnimationMap.find(clipName)==mAnimationMap.end()){
        std::cerr<<"update "<<clipName<<" error, no this animClip"<<std::endl;
    }
    AnimationClip clip=mAnimationMap[clipName];
    D3DAnim* anim=(D3DAnim*)D3DSourceManager::GetSourceManager()->GetSource(clip.orginAnimName).source;
    float startTick=clip.startTicks;
    float duration=clip.duration;

    float time=(GR_TIMER->TotalTime()-mStartTime)*anim->GetTicks();

    float ticks=time*speed;
    if(infinite){
        ticks=fmod(ticks,duration);
    }else{
        if(ticks>=startTick+duration){
            ticks=duration;
        }
    }
    ticks=ticks+startTick;
    UpdateModelMesh(mm,anim,ticks);

}

void AnimManager::UpdateModelMesh(GRCore::ModelMesh *mm, GRCore::D3DAnim *anim,float time) {

    UpdateModelNode(mm,anim,mm->GetRootNode(),time,DEFAULT_TEX_TRANSFORM);
}

void AnimManager::UpdateModelNode(GRCore::ModelMesh *mm, GRCore::D3DAnim *anim, std::string node, float time,
                                  const XMFLOAT4X4 &parent) {
    XMFLOAT4X4 trans=mm->GetNodeTransform(node);

    if(anim->HasAnim(node)){
        std::cout<<"get anim data"<<std::endl;
        trans=anim->GetInterpolatedTransform(node,time);
    }
    XMFLOAT4X4 result;
    XMMATRIX temp=XMLoadFloat4x4(&parent);
    XMMATRIX localTransform=XMLoadFloat4x4(&trans);
    temp=localTransform*temp;

    XMStoreFloat4x4(&result,temp);
    int idx;
    if((idx=mm->GetBoneIdx(node))!=-1){
        mm->SetBoneMatrix(result,idx);
    }

    //处理子节点
    std::vector<std::string> children=mm->GetNodeChildren(node);
    if(!children.empty()){
        for(auto iter:children){
            UpdateModelNode(mm,anim,iter,time,result);
        }
    }
}