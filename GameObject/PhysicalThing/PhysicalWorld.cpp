#include"PhysicalWorld.h"
#include "../GameObject.h"
#include "../../Component/Transform.h"
using namespace GRCore;
PhysicalWorld* GRCore::PhysicalWorld::mWorld=nullptr;

PhysicalWorld* PhysicalWorld::GetPhysicalWorld() {
    if(mWorld==nullptr){
        mWorld=new PhysicalWorld();
    }
    return mWorld;
}

void PhysicalWorld::Initialize(XMFLOAT3 start, XMFLOAT3 end) {
    mWorld=new PhysicalWorld(start,end);
}


PhysicalWorld::PhysicalWorld(XMFLOAT3 start, XMFLOAT3 end){
    this->start = start;
    this->end = end;
    octree = new OctreeNode(float(start.x), float(start.y), float(start.z), float(end.x), float(end.y), float(end.z), ROOT, 1, 3);
    terrain = new OctreeNode(float(start.x), float(start.y), float(start.z), float(end.x), float(end.y), float(end.z), ROOT, 1, 3);
    octree->BuildTree(1);
    terrain->BuildTree(1);
}

void PhysicalWorld::deleteObject(GRCore::Collider *collider) {
    if(removedCollider.count(collider)!=0){
        std::cerr<<"had removed before"<<std::endl;
        return;
    }
    removedCollider.insert(collider);
}

//判断是否碰撞了
void PhysicalWorld::update(float deltaTime){
    std::list<Collider*> tempList = octree->GetObjectsAt(start.x,start.y,start.z,end.x,end.y,end.z);
    std::cout<<"temp size:"<<tempList.size()<<std::endl;
    std::list<Collider*> allList;
    for(auto iter:tempList){
        if(removedCollider.count(iter)==0) {
            allList.push_back(iter);
            if (iter->isGravity) {
                std::cout << "update gravity:" << iter->GetGameObject()->GetName() << std::endl;
                Collider *c = iter;
                Transform * t = c->GetGameObject()->GetComponent<Transform>();
                XMFLOAT3 cp = t->GetPosition();
                t->SetPosition(cp.x, cp.y - (c->curSpeed * deltaTime), cp.z);
                c->curSpeed += 9.8f * deltaTime;
                if (t->GetPosition().y+c->offset.y <= (c->GetHeight()) / 2) {
                    t->SetPosition(cp.x, (c->GetHeight()) / 2-c->offset.y, cp.z);
                    c->curSpeed = .0f;
                }
            }
        }
    }
    removedCollider.clear();
    std::cout << "how small" << std::endl;
    std::cout << allList.size() << std::endl;
    //allList.unique();
    //allList.erase(unique(allList.begin(), allList.end()), allList.end());
    
    std::cout << "世界里面有几个collision"<<allList.size() << std::endl;
    octree->RemoveObjectsAt(start.x, start.y, start.z, end.x, end.y,end.z);
    std::list<Collider *>::iterator allIterator;
    for(allIterator=allList.begin();allIterator!=allList.end();++allIterator){
        octree->InsertObject(*allIterator);
    }
    updateCollision(octree->bottom_left_front_node, terrain->bottom_left_front_node);
    updateCollision(octree->bottom_right_front_node, terrain->bottom_right_front_node);
    updateCollision(octree->bottom_left_back_node, terrain->bottom_left_back_node);
    updateCollision(octree->bottom_right_back_node, terrain->bottom_right_back_node);
   updateCollision(octree->top_left_front_node, terrain->top_left_front_node);
    updateCollision(octree->top_right_front_node, terrain->top_right_front_node);
    updateCollision(octree->top_left_back_node, terrain->top_left_back_node);
    updateCollision(octree->top_right_back_node, terrain->top_right_back_node);

}
// 检测函数
void PhysicalWorld::updateCollision(OctreeNode *node, OctreeNode *terrain){
    soloUpdate(node,terrain, BOTTOM_LEFT_FRONT);
    soloUpdate(node,terrain, BOTTOM_RIGHT_FRONT);
    soloUpdate(node,terrain, BOTTOM_LEFT_BACK);
    soloUpdate(node,terrain, BOTTOM_RIGHT_BACK);
    soloUpdate(node,terrain,  TOP_LEFT_FRONT);
    soloUpdate(node,terrain, TOP_RIGHT_FRONT);
    soloUpdate(node,terrain, TOP_LEFT_BACK);
    soloUpdate(node,terrain, TOP_RIGHT_BACK);
}
void PhysicalWorld::soloUpdate(OctreeNode* node,OctreeNode* terrain, int area){
    std::list<Collider *> list = node->getYourDaughter(area);
    std::list<Collider *> listOfTerrain = terrain->getYourDaughter(area);

    //list.sort().unique();
    std::cout<<"area:"<<area<< " changdu " << listOfTerrain.size() << std::endl;
    list.erase(unique(list.begin(), list.end()), list.end());
    if (list.empty()) {
        return;
    }
    std::cout << "how big" << std::endl;
    std::cout << list.size() << std::endl;
    std::list<Collider *>::iterator iter;
    std::list<Collider *>::iterator iter1;
    std::list<Collider *>::iterator iter2;
    RectCollider* rect1 = nullptr;
    RectCollider* rect2 = nullptr;
    SphereCollider* sphere1 = nullptr;
    SphereCollider* sphere2 = nullptr;
    //iter1 = list.begin()++;
    //(*iter)->getBound().Intersects((*iter1)->getBound());
    for(iter=list.begin();iter!=list.end();++iter){
        std::cout<<"start clear collision list"<<std::endl;
        (*iter)->clearWhatICollide();
        std::cout<<"start process terrain:"<<listOfTerrain.size()<<std::endl;
        for(iter2=listOfTerrain.begin();iter2!=listOfTerrain.end();++iter2){
            sphere1=nullptr;
            rect1=nullptr;
            TriangleCollider* triangle= dynamic_cast<TriangleCollider*>(*iter2);
            if ((*iter)->getName() == "SphereCollider") {
                sphere1 = dynamic_cast<SphereCollider*>(*iter);
                rect1 = nullptr;
            }else if ((*iter)->getName() == "RectCollider") {
                sphere1 = nullptr;
                rect1 = dynamic_cast<RectCollider*>(*iter);
            }


            XMVECTOR t1,t2,t3;
            t1=XMLoadFloat3(&triangle->a1);
            t2=XMLoadFloat3(&triangle->a2);
            t3=XMLoadFloat3(&triangle->a3);

            //如果是方体发生碰撞
            if(sphere1==nullptr){
                if((rect1->getBound())->Intersects(t1,t2,t3)||(rect1->getBound())->Contains(t1,t2,t3)){
                    std::cout<<"rect collide terrain:"<<std::endl;
                    rect1->appendCollide(triangle);
                }
            }
            else{
                if((sphere1->getBound())->Intersects(t1,t2,t3)||(sphere1->getBound())->Contains(t1,t2,t3)){
                    std::cout<<"sphere collide terrain"<<std::endl;
                    sphere1->appendCollide(triangle);
                }
            }
        }
        for(iter1=list.begin();iter1!=list.end();++iter1){
            std::cout << "have you came in1" << std::endl;
            if(*iter==*iter1){
                continue;
            }
            std::cout << "have you came in2" << std::endl;
            std::cout << (*iter)->getName() << std::endl;
            if ((*iter)->getName() == "SphereCollider") {
                sphere1 = dynamic_cast<SphereCollider*>(*iter);
                rect1 = nullptr;
            }else if ((*iter)->getName() == "RectCollider") {
                sphere1 = nullptr;
                rect1 = dynamic_cast<RectCollider*>(*iter);
            }
            if ((*iter1)->getName() == "SphereCollider") {
                rect2 = nullptr;
                sphere2 = dynamic_cast<SphereCollider*>(*iter1);
            }
            else if ((*iter1)->getName() == "RectCollider") {
                rect2 = dynamic_cast<RectCollider*>(*iter1);
                sphere2 = nullptr;
            }
            std::cout << "gei wo jin lai" << std::endl;
            if (rect1 == nullptr) {
                if (sphere2 == nullptr) {
                    if ((sphere1->getBound())->Intersects(*(rect2)->getBound())) {
                        std::cout << "can you do" << std::endl;
                        sphere1->appendCollide(rect2);
                   }
                }
                else {
                    if ((sphere1->getBound())->Intersects(*(sphere2)->getBound())) {
                        std::cout << "can you do" << std::endl;
                        sphere1->appendCollide(sphere2);
                    }
                }    
            }
            else {
                if (sphere2 == nullptr) {
                    if ((rect1->getBound())->Intersects(*(rect2)->getBound())) {
                        std::cout << "can you do" << std::endl;
                        rect1->appendCollide(rect2);
                    }
                }
                else {
                    if ((rect1->getBound())->Intersects(*(sphere2)->getBound())) {
                        std::cout << "can you do" << std::endl;
                        rect1->appendCollide(sphere2);
                    }
                }
            }
            /*if(((*iter)->getBound())->Intersects(*(*iter1)->getBound())){
                (*iter)->appendCollide((*iter1));
            }*/
        }
        std::cout<<"a collider processed finished"<<std::endl;
    }
}
//std::vector<Collider *> PhysicalWorld::radioDetect(XMFLOAT3 position,XMFLOAT3 direction){
//    return;
//}