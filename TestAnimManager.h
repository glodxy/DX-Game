//
// Created by glodxy on 2019/12/23.
//

#ifndef GAME_TESTANIMMANAGER_H
#define GAME_TESTANIMMANAGER_H

#include "Component/ModelMesh.h"
#include "App/D3DAnim.h"

namespace GRCore {
    class TestAnimManager {
    public:

        void UpdateModelMesh(ModelMesh *mm,D3DAnim*anim){
            std::cout<<"ticks:"<<anim->GetTicks()<<std::endl;
            std::cout<<"duration:"<<anim->GetDuration()<<std::endl;
            float time=GR_TIMER->TotalTime()*anim->GetTicks();
            std::cout<<"time:"<<time<<std::endl;
            float animTime=fmod(time,anim->GetDuration());
            std::cout<<"anim time:"<<animTime<<std::endl;
            UpdateModelMeshNode(mm,anim,mm->GetRootNode(),animTime,DEFAULT_TEX_TRANSFORM);
        }


    private:
        void UpdateModelMeshNode(ModelMesh*mm,D3DAnim*anim,std::string node,float time,const XMFLOAT4X4& parent){
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
                    UpdateModelMeshNode(mm,anim,iter,time,result);
                }
            }

        }
    };
}

#endif //GAME_TESTANIMMANAGER_H
