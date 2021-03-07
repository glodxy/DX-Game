//
// Created by 田淙宇 on 2019/12/16.
//

#ifndef GAME_COLLIDER_H
#define GAME_COLLIDER_H

#include "../BaseComponent.h"
#include <Directxcollision.h>
namespace GRCore {
    class PhysicalWorld;
    class Collider:public  BaseComponent{
        
    public:
        Collider() {
            x = 0;
            y = 0;
            z = 0;
        }
        Collider(bool isTrigger,
            float x, float y, float z,
            bool isStatic);
        virtual void update() {
            std::cout<<"base collider called"<<std::endl;
        };
        void clearWhatICollide();
        virtual std::string getName() const{
            return "Collider";
        }
        //BoundingSphere* getBound();
        XMFLOAT3 getPosition();
        void setPosition(float x, float y, float z);
        void appendCollide(Collider* c);
        virtual float GetHeight()const{
            return 0;
        };
        virtual bool IsCollission(Collider* collider) { return false; }
        virtual void SetBoundCenter(XMFLOAT3 center){}
        virtual XMFLOAT3 GetBoundCenter(){return XMFLOAT3(0,0,0);}
        //float getRadius();
        float x;
        float y;
        float z;

        XMFLOAT3 offset;
        //float radius;
        bool isGravity;
        float curSpeed;
    protected:
        std::vector<Collider *> whatICollide;
        bool isTrigger;
        bool isStatic;

    };
    class RectCollider :public Collider {
    public:
        RectCollider(bool isTrigger,
            float x, float y, float z, XMFLOAT3 extent,
            bool isStatic, bool isGravity);
        XMFLOAT3 extent;
        BoundingBox* box;
        BoundingBox* getBound();

        XMFLOAT3 getExtent() { return extent; }
        void update();
        void OnAddOnObject(GameObject*obj);
        float GetHeight()const{
            return box->Extents.y*2;
        }
        void SetBoundCenter(XMFLOAT3 center){
            std::cout<<"rect set bound center called"<<std::endl;
            box->Center=center;
            std::cout<<"set bound center over"<<std::endl;
        }
        XMFLOAT3 GetBoundCenter(){
            return box->Center;
        }
        std::string getName() const {
            return "RectCollider";
        }
    };
    class SphereCollider : public Collider {
    public:
        SphereCollider(bool isTrigger,
            float x, float y, float z, float radius,
            bool isStatic, bool isGravity);
        float radius;
        BoundingSphere* sphere;
        float getRadius();
        void update();
        BoundingSphere* getBound();
        void OnAddOnObject(GameObject*obj);
        float GetHeight()const{
            return sphere->Radius*2;
        }
        void SetBoundCenter(XMFLOAT3 center){
            sphere->Center=center;
        }
        XMFLOAT3 GetBoundCenter(){
            return sphere->Center;
        }
        std::string getName() const {
            return "SphereCollider";
        }
    };
    class TriangleCollider : public Collider {
    public:
        XMFLOAT3 a1;
        XMFLOAT3 a2;
        XMFLOAT3 a3;
        TriangleCollider(XMFLOAT3 a1, XMFLOAT3 a2, XMFLOAT3 a3);

        std::string getName() const {
            return "TriangleCollider";
        }

    };
    class TerrainCollider : public Collider {
    public :
        void Init(XMFLOAT3* vertex, unsigned short* index, int vCount, int iCount, PhysicalWorld* world);
        void OnAddOnObject(GameObject*obj);
        std::string getName()const{
            return "TerrainCollider";
        }
    };
}



#endif //GAME_COLLIDER_H
