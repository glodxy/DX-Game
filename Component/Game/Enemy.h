//
// Created by glodxy on 2019/12/27.
//

#ifndef GAME_ENEMY_H
#define GAME_ENEMY_H

#include "../BaseComponent.h"

namespace GRCore {
    class Enemy:public BaseComponent{
    public:
        Enemy(int type){
            mType=type;
            mDead=false;
        }
        void update();
        void OnCollision(std::vector<GameObject*> collisions);

        std::string getName()const{
            return "Enemy";
        }

        void OnAddOnObject(GameObject*obj);
    private:
        bool mDead=false;
        float mDeadTime;
        int dir;
        float mSpeed;
        float mStartTime;
        int mType;
    };
}


#endif //GAME_ENEMY_H
