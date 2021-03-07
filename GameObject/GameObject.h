//
// Created by 田淙宇 on 2019/12/3.
//

#ifndef GAME_GAMEOBJECT_H
#define GAME_GAMEOBJECT_H

#include "../Component/BaseComponent.h"
#include <vector>

namespace GRCore {
    class Node;
    class Scene;
    class Collider;

    extern void DestroyObject(Scene*scene,GameObject*obj);

    class GameObject {
        template<typename T , typename TBase> class IsDerived
        {
        public:
            static int t(TBase* base)
            {
                return 1;
            }
            static  char t(void* t2)
            {
                return 0;
            }

            enum
            {
                Result = ( sizeof(int) == sizeof(t( (T*)NULL) )  ),
            };
        };
    public:
        GameObject();
        virtual ~GameObject(){
            for(auto iter:mComList){
                delete iter;
            }
        };

        //添加组件
        void AddComponent(BaseComponent* component);
        //移除组件
        void RemoveComponent(BaseComponent*component);

        //设置姓名
        void SetName(const std::string& name){
            mName=name;
        }
        //设置tag
        void SetTag(const std::string& tag){
            mTag=tag;
        }

        //获取姓名
        std::string GetName()const{
            return mName;
        }
        //获取tag
        std::string GetTag()const{
            return mTag;
        }

        RenderType GetRenderType()const{
            return mRenderType;
        }

        Scene* GetScene(){
            return mScene;
        }
/////////////此接口勿调用！！！！
        void SetScene(Scene* scene){
            mScene=scene;
        }
////////////勿调！！！！！
        void SetNode(Node* node){
            mNode=node;
        }

        //获取该物体对应的scene中的节点
        Node* GetNode(){
            return mNode;
        }

        //为该物体添加子物体
        void AddChild(GameObject*obj);
        //为该物体添加父物体
        void SetParent(GameObject* obj);


//此处勿调！！！！！！！！！
        void fixedUpdate();
        void update();
        void awake();
        void destroy();

        virtual void OnCollision(std::vector<Collider*>colliders);

        virtual void OnResize();
        virtual void OnKeyDown(char key);
        virtual void OnKeyUp(char key);

        template <class T>
        T* GetComponent(){
            T *a;
            for(int i=0;i<mComList.size();++i){
                if(typeid(*(mComList[i]))== typeid(T)){
                    return dynamic_cast<T*>(mComList[i]);
                }
            }
            return nullptr;
        }

        template <class T>
        const T* GetComponent()const{
            T *a;
            for(int i=0;i<mComList.size();++i){
                if(typeid(*(mComList[i]))== typeid(T)){
                    return dynamic_cast<T*>(mComList[i]);
                }
            }
            return nullptr;
        }

        const std::unordered_map<std::string,ExtraData>& GetExtraDatas()const{
            return mExtraDataMap;
        }

        void SetExtraData(std::string name,ExtraData data){

            std::cout<<"add extradata:"<<name<<std::endl;
            if(mExtraDataMap.find(name)!=mExtraDataMap.end()){
                mExtraDataMap[name].ClearData();
            }
            mExtraDataMap[name]=data;
        }

        ExtraData GetExtraData(const std::string&name){
            if(mExtraDataMap.find(name)==mExtraDataMap.end()){
                std::cerr<<"error get extra data:"<<name<<" not exsit"<<std::endl;
                exit(0);
            }
            return mExtraDataMap[name];
        }

        int GetLayerMask()const{
            return mLayerMask;
        }

        void SetLayer(int layer){
            mLayerMask=layer;
        }

    private:
        int mLayerMask;

        std::unordered_map<std::string,ExtraData> mExtraDataMap;

    protected:
        std::string mName="none";
        std::string mTag="none";
        Node* mNode;
        Scene*mScene;
        std::vector<BaseComponent*> mComList;
        RenderType  mRenderType;
    };
}


#endif //GAME_GAMEOBJECT_H
