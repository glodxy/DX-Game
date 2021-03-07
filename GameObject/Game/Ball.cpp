//
// Created by glodxy on 2019/12/24.
//

#include "Ball.h"
#include "../../Component/Transform.h"
#include "../../Component/Mesh.h"
#include "../../Component/Game/BallController.h"
#include "../../Component/Collider/Collider.h"



using namespace GRCore;
Ball::Ball(XMFLOAT3 pos,XMFLOAT3 dir, int ballType, float speed):GameObject() {
    mDirection=dir;
    mSpeed=speed;
    GetComponent<Transform>()->SetScale(.3f,.3f,.3f);
    GetComponent<Transform>()->SetPosition(pos.x,pos.y,pos.z);
    Mesh* mesh=new Mesh(DEFAULT_SPHERE_MESH_NAME);
    mesh->SetShaderName(DEFAULT_SHADER_NAME);
    AddComponent(mesh);
    AddComponent(new BallController());
    AddComponent(new RectCollider(false,0,0,0,XMFLOAT3(.3F,.3F,.3F),false,true));
    mStartTime=GR_TIMER->TotalTime();
    this->ballType = ballType;
    if (ballType==WATERBALL) {
        mesh->SetMaterialName("water");
    }else if (ballType == FIREBALL) {
        mesh->SetMaterialName("fire");
    }
    else {
        mesh->SetMaterialName("green");
    }
}

XMFLOAT3 Ball::GetDirection() const {
    return mDirection;
}

float Ball::GetSpeed() const {
    return mSpeed;
}

void Ball::OnCollision(std::vector<Collider *> colliders) {
    if(!colliders.empty()) {
        for(auto iter:colliders){
            if((iter)->getName()=="SphereCollider"||(iter)->getName()=="RectCollider"){
                if(iter->GetGameObject()->GetName()!="majorModel"){
                    std::cout << "ball collision called" << std::endl;
                    DestroyObject(mScene, this);
                }

            }
        }

    }
}