//
// Created by glodxy on 2019/12/22.
//

#ifndef GAME_WATERCONTROLLER_H
#define GAME_WATERCONTROLLER_H

#include "BaseComponent.h"

namespace GRCore {
    class WaterController: public BaseComponent{
    public:
        WaterController(){}

        void update()final;

        std::string getName()const{
            return "WaterController";
        }
    protected:

    };
}


#endif //GAME_WATERCONTROLLER_H
