//
// Created by glodxy on 2019/12/24.
//

#ifndef GAME_ANIMMANAGER_H
#define GAME_ANIMMANAGER_H

#include "D3DUtil.h"
#include "D3DAnim.h"

namespace GRCore {
    class ModelMesh;
    class AnimManager {
        typedef struct{
            std::string orginAnimName;
            float startTicks;
            float duration;
        }AnimationClip;
    public:
        static AnimManager* GetAnimManager();

        bool LoadAnimationClip(std::string clipName,std::string originName,float startTicks,float duration);

        void ResetAnimCount(){
            mStartTime=GR_TIMER->TotalTime();
            mCount=0;
        }

        void UpdateAnim(ModelMesh*mm,const std::string&clipName,float speed,bool infinite);
    private:
        void UpdateModelMesh(ModelMesh*mm,D3DAnim* anim,float time);

        void UpdateModelNode(ModelMesh*mm,D3DAnim*anim,std::string node,float time,const XMFLOAT4X4& parent);
        AnimManager();
        static AnimManager* mAnimManager;
        std::map<std::string,AnimationClip> mAnimationMap;
        float mStartTime;
        int mCount=0;
    };
}


#endif //GAME_ANIMMANAGER_H
