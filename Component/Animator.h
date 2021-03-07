//
// Created by glodxy on 2019/12/24.
//

#ifndef GAME_ANIMATOR_H
#define GAME_ANIMATOR_H

#include "BaseComponent.h"

namespace GRCore {
    class Animator:public BaseComponent{
        typedef struct{
            std::string animName;
            float speed;
            bool infinite;
        }Animation;
    public:
        Animator();

        void AddAnimation(std::string stateName,std::string animClipName,float speed,bool infinite);

        void update()final;

        void SetCurState(std::string state);
        std::string GetCurState()const{
            return mCurState;
        }

        void SetInterval(int frames);

        std::string getName()const{
            return "Animator";
        }
    protected:
        int mIntervalFrame,mCurFrame;
        std::string mCurState;//当前状态
        std::map<std::string,Animation> mAnimStates;
    };
}


#endif //GAME_ANIMATOR_H
