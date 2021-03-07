//
// Created by glodxy on 2019/12/24.
//

#ifndef GAME_BALL_H
#define GAME_BALL_H
#define WATERBALL 1
#define FIREBALL 2
#define GRASSBALL 3
#include "../GameObject.h"

namespace GRCore {
    class Ball :public GameObject{
    public:
        Ball(XMFLOAT3 pos,XMFLOAT3 dir, int ballType, float speed=1.0f);
        XMFLOAT3 GetDirection()const;
        float GetSpeed()const;
        virtual ~Ball()=default;

        void OnCollision(std::vector<Collider*> colliders);
        float mStartTime;
        int ballType;
    protected:

        float mSpeed;
        XMFLOAT3 mDirection;

    };
}


#endif //GAME_BALL_H
