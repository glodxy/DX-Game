//
// Created by glodxy on 2019/12/23.
//

#ifndef GAME_D3DANIM_H
#define GAME_D3DANIM_H

#include "D3DUtil.h"
#include <map>
namespace GRCore {
    class D3DAnim {
    public:
        bool Initialize(D3DAnimMap pos,D3DAnimMap rotate,D3DAnimMap scale,D3DAnimStateMap states,float ticks,float duration);
        void LoadAnim(std::string nodeName,std::vector<AnimData> posAnim,std::vector<AnimData> rotateAnim,std::vector<AnimData> scaleAnim,NodeAnimState state);


        bool HasAnim(std::string node);

        XMFLOAT4X4 GetInterpolatedTransform(std::string node,float time);

        XMFLOAT4 CalcInterpolatedScaling(std::string node,float time);
        XMFLOAT4 CalcInterpolatedPosition(std::string node,float time);
        XMFLOAT4 CalcInterpolatedRotation(std::string node,float time);

        int FindDataByTimeFloor(float time,std::vector<AnimData> data);
        int FindDataByTimeUp(float time,std::vector<AnimData> data);

        void PrintData(std::string node);

        float GetTicks()const{
            return mTicks;
        }

        float GetDuration()const{
            return mDuration;
        }

    private:
        D3DAnimMap mPositionAnims;
        D3DAnimMap mRotationAnims;
        D3DAnimMap mScaleAnims;
        D3DAnimStateMap mAnimChangeState;
        float mTicks;
        float mDuration;
    };
}


#endif //GAME_D3DANIM_H
