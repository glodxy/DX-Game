//
// Created by 田淙宇 on 2019/12/11.
//

#ifndef GAME_CAMERACONTROLLER_H
#define GAME_CAMERACONTROLLER_H

#include "BaseComponent.h"

namespace GRCore{
    class CameraController:public BaseComponent{
    public:
        CameraController(float distance,GameObject*obj);
        void update();

        std::string getName()const{
            return "camera controller";
        }
    protected:
        GameObject* mTarget;
        float mDistance;
        bool mPressed=false;
        float mLastMouseX;
        float mLastMouseY;
    };
}
#endif //GAME_CAMERACONTROLLER_H
