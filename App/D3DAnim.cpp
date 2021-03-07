//
// Created by glodxy on 2019/12/23.
//

#include "D3DAnim.h"

using namespace GRCore;

bool D3DAnim::Initialize(GRCore::D3DAnimMap pos, GRCore::D3DAnimMap rotate, GRCore::D3DAnimMap scale,
                         GRCore::D3DAnimStateMap states,float ticks,float duration) {
    mPositionAnims=pos;
    mRotationAnims=rotate;
    mScaleAnims=scale;
    mAnimChangeState=states;
    mTicks=ticks;
    mDuration=duration;
    return true;
}

bool D3DAnim::HasAnim(std::string node) {
    if(mPositionAnims.find(node)==mPositionAnims.end()){
        return false;
    }
    return true;
}

int D3DAnim::FindDataByTimeFloor(float time, std::vector<AnimData> data) {
    if(data.size()<1){
        std::cerr<<"cannot find animdata, data list empty"<<std::endl;
    }
    //找到time之下最大的值
    for(int i=0;i<data.size()-1;++i){
        if(time<data[i+1].time){
            return i;
        }
    }
    return 0;
}

int D3DAnim::FindDataByTimeUp(float time, std::vector<AnimData> data) {
    if(data.size()<1){
        std::cerr<<"cannot find animdata, data list empty"<<std::endl;
    }
    for(int i=0;i<data.size();++i){
        if(data[i].time>=time){
            return i;
        }
    }
    return data.size()-1;
}

void D3DAnim::LoadAnim(std::string nodeName, std::vector<AnimData> posAnim, std::vector<AnimData> rotateAnim,
                       std::vector<AnimData> scaleAnim, GRCore::NodeAnimState state) {
    mPositionAnims[nodeName]=posAnim;
    mRotationAnims[nodeName]=rotateAnim;
    mScaleAnims[nodeName]=scaleAnim;
    mAnimChangeState[nodeName]=state;
}

XMFLOAT4 D3DAnim::CalcInterpolatedScaling(std::string node,float time) {
    if(mScaleAnims.find(node)==mScaleAnims.end()){
        return XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
    }
    std::vector<AnimData> anims=mScaleAnims[node];

    int beginIndex=FindDataByTimeFloor(time,anims);
    int endIndex=beginIndex+1;

    AnimData beginData=anims[beginIndex];
    AnimData endData=anims[endIndex];

    std::cout<<"scale anim begin time:"<<beginData.time<<std::endl;
    std::cout<<"scale anim end time:"<<endData.time<<std::endl;
    float deltaTime=endData.time-beginData.time;
    float factor;
    factor=beginIndex==0?0:(time-beginData.time)/deltaTime;
    XMVECTOR s=XMLoadFloat4(&beginData.transform);
    XMVECTOR e=XMLoadFloat4(&endData.transform);

    std::cout<<"scale anim start value:"<<s<<std::endl;
    std::cout<<"scale anim end value:"<<e<<std::endl;

    XMVECTOR r=XMVectorLerp(s,e,factor);
    XMFLOAT4 result;
    XMStoreFloat4(&result,r);
    return result;
}

XMFLOAT4 D3DAnim::CalcInterpolatedPosition(std::string node, float time) {
    if(mPositionAnims.find(node)==mPositionAnims.end()){
        std::cerr<<"cannot find pos anim of "<<node<<std::endl;
        return XMFLOAT4(.0f,.0f,.0f,1.0f);
    }
    std::vector<AnimData> anims=mPositionAnims[node];

    int beginIndex=FindDataByTimeFloor(time,anims);
    int endIndex=beginIndex+1;

    AnimData beginData=anims[beginIndex];
    AnimData endData=anims[endIndex];

    std::cout<<"pos anim begin time:"<<beginData.time<<std::endl;
    std::cout<<"pos anim end time:"<<endData.time<<std::endl;


    float deltaTime=endData.time-beginData.time;
    float factor;
    factor=beginIndex==0?0:(time-beginData.time)/deltaTime;
    XMVECTOR s=XMLoadFloat4(&beginData.transform);
    XMVECTOR e=XMLoadFloat4(&endData.transform);

    std::cout<<"pos anim start value:"<<s<<std::endl;
    std::cout<<"pos anim end value:"<<e<<std::endl;

    XMVECTOR r=XMVectorLerp(s,e,factor);
    XMFLOAT4 result;
    XMStoreFloat4(&result,r);
    return result;
}

XMFLOAT4 D3DAnim::CalcInterpolatedRotation(std::string node, float time) {
    if(mRotationAnims.find(node)==mRotationAnims.end()){
        XMFLOAT4 temp;
        XMVECTOR t=XMQuaternionIdentity();
        XMStoreFloat4(&temp,t);
        return temp;
    }
    std::vector<AnimData> anims=mRotationAnims[node];

    int beginIndex=FindDataByTimeFloor(time,anims);
    int endIndex=beginIndex+1;

    AnimData beginData=anims[beginIndex];
    AnimData endData=anims[endIndex];

    std::cout<<"pos anim begin time:"<<beginData.time<<std::endl;
    std::cout<<"pos anim end time:"<<endData.time<<std::endl;

    float deltaTime=endData.time-beginData.time;
    float factor;
    factor=beginIndex==0?0:(time-beginData.time)/deltaTime;
    XMVECTOR s=(XMLoadFloat4(&beginData.transform));
    XMVECTOR e=(XMLoadFloat4(&endData.transform));

    std::cout<<"rotation anim start value:"<<s<<std::endl;
    std::cout<<"rotation anim end value:"<<e<<std::endl;

    XMVECTOR r=XMQuaternionNormalize(XMQuaternionSlerp(s,e,factor));
    XMFLOAT4 result;
    XMStoreFloat4(&result,r);
    return result;
}

XMFLOAT4X4 D3DAnim::GetInterpolatedTransform(std::string node, float time) {
    XMMATRIX scale,translation,rotation;
    XMFLOAT4 s,t,r;
    s=CalcInterpolatedScaling(node,time);
    t=CalcInterpolatedPosition(node,time);
    r=CalcInterpolatedRotation(node,time);
    scale=XMMatrixScaling(s.x,s.y,s.z);
    translation=XMMatrixTranslation(t.x,t.y,t.z);

    XMVECTOR rotationQuaternion=XMLoadFloat4(&r);
    rotation=XMMatrixRotationQuaternion(rotationQuaternion);

    XMFLOAT4X4 result;
    XMMATRIX temp=scale*rotation*translation;
    XMStoreFloat4x4(&result,temp);
    return result;
}