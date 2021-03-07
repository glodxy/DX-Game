//
// Created by 田淙宇 on 2019/12/16.
//

#include "Collider.h"
#include"../Transform.h"
#include"../../GameObject/GameObject.h"
#include"../../GameObject/PhysicalThing/PhysicalWorld.h"
#include "../../App/D3DSourceManager.h"
#include "../Mesh.h"

using namespace GRCore;
float calculate(float x, float y, float z) {
    return (x * x + y * y) / (x * x + y * y + z * z);
}
bool upDown(float x, float y, float z, float w, float h, float d) {
    return (calculate(x, z, y) <= calculate(w, d, h));
}
bool leftRight(float x, float y, float z, float w, float h, float d) {
    return calculate(x, 0, z) <= calculate(w, 0, d);
}
float theLength(float x, float y, float z, float w, float h, float d) {
    if (upDown(x, y, z, w, h, d)) {
        return (h)*sqrt((x * x) + (y * y) + (z * z))/abs(y);
    }
    else if (leftRight(x, y, z, w, h, d)) {
        return d * sqrt(x * x + y * y + z * z) / abs(z);
    }
    else {
        return w * sqrt(x * x + y * y + z * z) / abs(x);
    }
}
//bool beyondBack();
Collider::Collider(bool isTrigger,
        float x, float y, float z,
        bool isStatic){
            this->isTrigger = isTrigger;
            offset.x=x;
            offset.y=y;
            offset.z=z;

            //this->radius = radius;
            //this->sphere = new BoundingSphere(XMFLOAT3(x,y,z),radius);
            this->isStatic = isStatic;
            this->curSpeed=.0f;
            //whatICollide.resize(10);

        }
SphereCollider::SphereCollider(bool isTrigger,
    float x, float y, float z, float radius,
    bool isStatic,bool isGravity) :Collider(isTrigger,x,y,z,isStatic){
    this->radius = radius;
    this->isGravity = isGravity;
    this->sphere = new BoundingSphere(XMFLOAT3(x+offset.x, y+offset.y, z+offset.z), radius);
}
RectCollider::RectCollider(bool isTrigger,
    float x, float y, float z, XMFLOAT3 extent,
    bool isStatic,bool isGravity) :Collider(isTrigger, x, y, z, isStatic){
    this->extent = extent;
    this->isGravity = isGravity;
    this->box = new BoundingBox(XMFLOAT3(x+offset.x, y+offset.y, z+offset.z), extent);
}

float SphereCollider::getRadius(){
    return this->radius;
}
void RectCollider::update() {
    std::cout<<"rect collider called"<<std::endl;
    Transform* t = mObject->GetComponent<Transform>();
//    XMFLOAT3 position = t->GetPosition();
//    this->x = position.x;
//    this->y = position.y;
//    this->z = position.z;
//    box = new BoundingBox(XMFLOAT3(x, y, z), extent);
//    Transform* t=GetGameObject()->GetComponent<Transform>();
    XMFLOAT3 cp=t->GetPosition();
    std::cout << "myPosition" << x << y << z << std::endl;
    if (whatICollide.empty()) {
        std::cout << "It's empty" << std::endl;
        return;
    }
    if (isTrigger) {
        if (whatICollide.empty()) {
            return;
        }
        else {
            mObject->OnCollision(whatICollide);
            //mObject->onCollide(whatICollide);
            std::cout << "niude:" << std::endl;
            return;
        }
    }
    else {
        if (isStatic) {
            mObject->OnCollision(whatICollide);
            return;
        }

        std::cout << "boom!!!" << std::endl;

        bool influent=false;
        for (int i = 0; i < whatICollide.size(); i++) {
            XMFLOAT3 collidePosition = (whatICollide[i])->getPosition();
            if(whatICollide[i]->getName()=="TriangleCollider"&&!influent){
                std::cout<<"terrain set pos"<<std::endl;
                std::cout<<"test:"<<t->GetPosition().x<<std::endl;
                t->SetPosition(cp.x,cp.y+0.01f,cp.z);
                curSpeed=0;
                influent=true;
            }
            else if (whatICollide[i]->getName() == "SphereCollider") {
                float collideRadius = dynamic_cast<SphereCollider*>(whatICollide[i])->getRadius();
                //XMFLOAT3 collideExtent = dynamic_cast<RectCollider*>(whatICollide[i])->getExtent();

                float d1 = sqrt((collidePosition.x - x) * (collidePosition.x - x) +
                    (collidePosition.y - y) * (collidePosition.y - y)
                    + (collidePosition.z - z) * (collidePosition.z - z));
            
                
                float alphax = -(collidePosition.x - x) / d1;
                float alphay = -(collidePosition.y - y) / d1;
                float alphaz = -(collidePosition.z - z) / d1;
                float d2 = 0.01f * d1;
                //float d2 = 
                float finalX = this->x + d2 * alphax;
                float finalY = this->y + d2 * alphay;
                float finalZ = this->z + d2 * alphaz;
                //float finalY = collideExtent.y - 1.001 * alphay*(collideExtent.y - this->y);
                //float finalZ = collideExtent.z - 1.001 *alphaz* (collideExtent.z - this->z);
                setPosition(finalX, finalY, finalZ);
                t->SetPosition(finalX,finalY,finalZ);
            }
            else if (whatICollide[i]->getName() == "RectCollider") {
                std::cout<<"collide rect"<<std::endl;
                XMFLOAT3 collideExtent = dynamic_cast<RectCollider*>(whatICollide[i])->getExtent();
                
                //已经成功了的 但是甲方硬要改
                float d1 = sqrt((collidePosition.x - x) * (collidePosition.x - x) +
                    (collidePosition.y - y) * (collidePosition.y - y)
                    + (collidePosition.z - z) * (collidePosition.z - z));
            
                
                float alphax = -(collidePosition.x - x) / d1;
                float alphay = -(collidePosition.y - y) / d1;
                float alphaz = -(collidePosition.z - z) / d1;
                float d2 = 0.01f * d1;
                //float d2 = 
                float finalX = this->x + d2 * alphax;
                float finalY = this->y + d2 * alphay;
                float finalZ = this->z + d2 * alphaz;
                //float finalY = collideExtent.y - 1.001 * alphay*(collideExtent.y - this->y);
                //float finalZ = collideExtent.z - 1.001 *alphaz* (collideExtent.z - this->z);
                //setPosition(finalX, finalY, finalZ);
                t->SetPosition(finalX,finalY,finalZ);
            }
        }
        mObject->OnCollision(whatICollide);
        return;
    }
 }
void SphereCollider::update() {
    std::cout<<mObject->GetName()<<" sphere collider called"<<std::endl;
    Transform* t = mObject->GetComponent<Transform>();
//    XMFLOAT3 position = t->GetPosition();
//    this->x = position.x;
//    this->y = position.y;
//    this->z = position.z;
//    sphere = new BoundingSphere(XMFLOAT3(x, y, z), radius);

    XMFLOAT3 cp=t->GetPosition();
    std::cout << "myPosition" << x << y << z << std::endl;
    if (whatICollide.empty()) {
        std::cout << "It's empty" << std::endl;
        return;
    }
    if(isTrigger){
        if(whatICollide.empty()){
            return;
        }else {
            mObject->OnCollision(whatICollide);
            //mObject->onCollide(whatICollide);
            std::cout << "niude:" << std::endl;
            return;
        }
    }
    else {
        std::cout<<mObject->GetName()<<" into collision"<<std::endl;
        if(isStatic){
            mObject->OnCollision(whatICollide);
            return;
        }
        
        std::cout << "boom!!!" << std::endl;
        std::cout<<whatICollide.size()<<std::endl;
        for (int i = 0; i < whatICollide.size(); i++) {
            XMFLOAT3 collidePosition = (whatICollide[i])->getPosition();
            if(whatICollide[i]->getName()=="TriangleCollider"){
                std::cout<<"terrain set pos"<<std::endl;
                    t->SetPosition(cp.x,cp.y+0.01f,cp.z);
                    curSpeed=0;
            }
            else if (whatICollide[i]->getName() == "SphereCollider") {
                float collideRadius = dynamic_cast<SphereCollider*>(whatICollide[i])->getRadius();
                /*float finalX = collidePosition.x - 0.5;
                float finalY = collidePosition.y - 0.5;
                float finalZ = collidePosition.z;*/
                float d1 = sqrt((collidePosition.x - x) * (collidePosition.x - x) +
                    (collidePosition.y - y) * (collidePosition.y - y)
                    + (collidePosition.z - z) * (collidePosition.z - z));
                float d2 = this->radius + collideRadius;
                float finalX = collidePosition.x - d2 / d1 * (collidePosition.x - this->x);
                float finalY = collidePosition.y - d2 / d1 * (collidePosition.y - this->y);
                float finalZ = collidePosition.z - d2 / d1 * (collidePosition.z - this->z);
                setPosition(finalX, finalY, finalZ);
                t->SetPosition(finalX,finalY,finalZ);
            }
            else if (whatICollide[i]->getName() == "RectCollider") {
                //XMFLOAT3 coolideExtent = dynamic_cast<RectCollider*>(whatICollide[i])->getExtent();
                XMFLOAT3 collideExtent = dynamic_cast<RectCollider*>(whatICollide[i])->getExtent();

                //已经成功了的 但是甲方硬要改
                float d1 = sqrt((collidePosition.x - x) * (collidePosition.x - x) +
                    (collidePosition.y - y) * (collidePosition.y - y)
                    + (collidePosition.z - z) * (collidePosition.z - z));


                float alphax = -(collidePosition.x - x) / d1;
                float alphay = -(collidePosition.y - y) / d1;
                float alphaz = -(collidePosition.z - z) / d1;
                float d2 = 0.01 * d1;
                //float d2 = 
                float finalX = this->x + d2 * alphax;
                float finalY = this->y + d2 * alphay;
                float finalZ = this->z + d2 * alphaz;
                //float finalY = collideExtent.y - 1.001 * alphay*(collideExtent.y - this->y);
                //float finalZ = collideExtent.z - 1.001 *alphaz* (collideExtent.z - this->z);
                setPosition(finalX, finalY, finalZ);
                t->SetPosition(finalX,finalY,finalZ);
            }
        }
        mObject->OnCollision(whatICollide);
        return ;
    }   
}
BoundingSphere* SphereCollider::getBound(){
    return this->sphere;
}
BoundingBox* RectCollider::getBound() {
    return this->box;
}
void Collider::setPosition(float x, float y, float z){
    std::cout<<"inside setposition"<<std::endl;
    std::cout<<offset.x<<std::endl;
    this->x = x+offset.x;
    std::cout<<offset.y<<std::endl;
    this->y = y+offset.y;
    std::cout<<offset.z<<std::endl;
    this->z = z+offset.z;
    std::cout << "what are you" << x << " " << y<<" " << " "<<z<<std::endl;
}
XMFLOAT3 Collider::getPosition(){
    //float Position[3] = {this->x, this->y, this->z};
    return XMFLOAT3(x,y,z);
}
void Collider::clearWhatICollide() {
    whatICollide.clear();
}
void Collider::appendCollide(Collider* c){
    whatICollide.push_back(c);
}
TriangleCollider::TriangleCollider(XMFLOAT3 a1, XMFLOAT3 a2, XMFLOAT3 a3) {
    this->a1 = a1;
    this->a2 = a2;
    this->a3 = a3;
}
void TerrainCollider::Init(XMFLOAT3* vertex, unsigned short* index, int vCount, int iCount, PhysicalWorld* world) {
    for (int i = 0; i < iCount - 2; i++) {
        world->terrain->InsertObject(new TriangleCollider(
            vertex[index[i]],
            vertex[index[i + 1]],
            vertex[index[i + 2]]));
    }
}


void RectCollider::OnAddOnObject(GRCore::GameObject *obj) {
    XMFLOAT3 pos= obj->GetComponent<Transform>()->GetPosition();
    this->x=pos.x;
    this->y =pos.y;
    this->z =pos.z;
    PhysicalWorld::GetPhysicalWorld()->octree->InsertObject(this);
}
void SphereCollider::OnAddOnObject(GRCore::GameObject *obj) {
    XMFLOAT3 pos= obj->GetComponent<Transform>()->GetPosition();
    this->x=pos.x;
    this->y =pos.y;
    this->z =pos.z;
    PhysicalWorld::GetPhysicalWorld()->octree->InsertObject(this);
}
void TerrainCollider::OnAddOnObject(GRCore::GameObject *obj) {

    MeshData md=D3DSourceManager::GetSourceManager()->GetMeshData(obj->GetComponent<Mesh>()->GetMeshName());
    Init(md.v,md.i,md.vertex_count,md.index_count,PhysicalWorld::GetPhysicalWorld());
}
