//
// Created by glodxy on 2019/12/25.
//

#ifndef GAME_BALLCONTROLLER_H
#define GAME_BALLCONTROLLER_H

#include "../BaseComponent.h"

namespace GRCore {
    class BallController:public BaseComponent{
    public:
        void update();

        std::string getName()const{
            return "BallController";
        }
    protected:
    };
}


#endif //GAME_BALLCONTROLLER_H
