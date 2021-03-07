//
// Created by glodxy on 2019/12/25.
//

#ifndef GAME_CHARACTERCONTROLLER_H
#define GAME_CHARACTERCONTROLLER_H

#include "../BaseComponent.h"

namespace GRCore {
    class CharacterController:public BaseComponent{
        enum CharacterState{
            IDLE,
            WALK,
            ATTACK,
            RUN
        };
    public:
        void Set(XMFLOAT3 front, XMFLOAT3 right);
        CharacterController(float speed);
        void update();

        void SetDirection(XMFLOAT3 front){
            mDirection=front;
        }
        void SetRight(XMFLOAT3 right){
            mRight=right;
        }

        std::string getName()const{
            return "CharacterController";
        }

    protected:
        float mSpeed;
        XMFLOAT3 mDirection;
        XMFLOAT3 mRight;
        CharacterState mState;
        float currTime;
        int count;
        XMFLOAT3 mFront;
    };
}


#endif //GAME_CHARACTERCONTROLLER_H
