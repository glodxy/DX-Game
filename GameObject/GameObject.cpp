//
// Created by 田淙宇 on 2019/12/3.
//

#include "GameObject.h"
#include "../Component/Transform.h"
#include "../Component/Collider/Collider.h"
#include "Scene.h"
using namespace GRCore;

GameObject::GameObject() {
    AddComponent(new Transform());
    mRenderType=RT_Geometry;
    mLayerMask=GR_DEFAULT_LAYER;
}

void GameObject::update() {
    for(int i=0;i<mComList.size();++i){
        mComList[i]->update();
    }
}

void GameObject::fixedUpdate() {
    for(int i=0;i<mComList.size();++i){
        mComList[i]->fixedUpdate();
    }
}

void GameObject::OnResize() {
    for(auto i:mComList){
        i->OnResize();
    }
}

void GameObject::OnKeyDown(char key) {
    for(auto i:mComList){
        i->OnKeyDown(key);
    }
}

void GameObject::OnKeyUp(char key) {
    for(auto i:mComList){
        i->OnKeyUp(key);
    }
}

void GameObject::OnCollision(std::vector<Collider *> colliders) {
    std::vector<GameObject*> objs;
    for(int i=0;i<colliders.size();++i){
        objs.push_back(colliders[i]->GetGameObject());
    }
    for(auto iter:mComList){
        iter->OnCollision(objs);
    }
}

void GameObject::AddComponent(GRCore::BaseComponent *component) {
    component->SetGameObject(this);
    component->OnAddOnObject(this);
    mComList.push_back(component);
}

void GameObject::RemoveComponent(GRCore::BaseComponent *component) {
    component->SetGameObject(nullptr);
    for(auto iter=mComList.begin();iter!=mComList.end();){
        if((*iter)==component){
            delete (*iter);
            mComList.erase(iter);
            break;
        }else{
            ++iter;
        }
    }
}


void GameObject::awake() {
    for(auto iter:mComList){
        iter->awake();
    }
}

void GameObject::destroy() {
    for(auto iter:mComList){
        iter->destroy();
    }
}

void GameObject::SetParent(GRCore::GameObject *obj) {
    mScene->SetParent(obj->mNode,this->mNode);
}

void GameObject::AddChild(GRCore::GameObject *obj) {
    mScene->AddChild(this->mNode,obj);
}