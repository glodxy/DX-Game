//
// Created by 田淙宇 on 2019/12/3.
//

#ifndef GAME_BASECOMPONENT_H
#define GAME_BASECOMPONENT_H

#include <iostream>
#include "../App/D3DUtil.h"

namespace GRCore{
    class GameObject;
    class BaseComponent{
    public:
        virtual ~BaseComponent()=default;
        //重写
        virtual void awake(){}
        virtual void create(){}
        //重写
        virtual void update(){
        }
        //重写
        virtual void fixedUpdate(){}
        //重写
        virtual void destroy(){}


        virtual void OnCollision(std::vector<GameObject*> colliders){}
        virtual void OnAddOnObject(GameObject*obj){}
//不管！！！！
        virtual void OnResize(){}
        virtual void OnKeyDown(char key){}
        virtual void OnKeyUp(char key){}

        //必须重写
        virtual std::string getName()const=0;//获取组件名
//不管！！！！！！！！！！
        void SetGameObject(GameObject* obj){
            mObject=obj;
        }

        //获取所挂的GameObject
        GameObject* GetGameObject(){
            return mObject;
        }
    protected:
        GameObject* mObject=nullptr;

    };
}
#endif //GAME_BASECOMPONENT_H
