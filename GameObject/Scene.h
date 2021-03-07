//
// Created by 田淙宇 on 2019/12/11.
//

#ifndef GAME_SCENE_H
#define GAME_SCENE_H


#include <set>
#include "Camera/Camera.h"
#include "../Component/Mesh.h"
#include "Light/BaseLight.h"
#include "../Component/SpriteList.h"
#include "../Component/Sprite.h"
#include "../Component/ModelMesh.h"


namespace GRCore {
    struct Node{
        Node * parent;
        GameObject* obj;
        std::vector<Node*> children;
    };


    class Scene {
    public:
        Scene();

        void AddCamera(Camera* camera){
            mCamera.push_back(camera);
        }

        void AddLight(BaseLight* light);

        //将obj添加为root的子物体
        void AddChild(Node* root,GameObject* obj);
        //将名字为name的obj从root的子物体中删除
        void RemoveChildByName(Node* root,std::string name);
        //找到名字为name的GameObject
        GameObject* FindGameObjectByName(std::string name);
        //找到root下名字为name的子GameObject
        GameObject* FindChildByName(Node*root,std::string name);
        Camera* FindCameraByName(std::string name);
        //将child的parent设为parent
        void SetParent(Node* parent,Node* child);

        //设置物体的渲染类型
        void SetRenderType(GameObject*obj,RenderType type);


        //打印物体及子物体姓名信息
        void PrintObj(GameObject*obj);

        void DestroyObject(GameObject*obj);


        void Awake();
        void Destroy();
        void Update();
        void FixedUpdate();
    private:
        void DestroyRemovedObject();

        void ReleaseNode(Node *node);
        GameObject* SearchByName(Node* root,std::string name);
        void RemoveFromRenderQueue(GameObject*obj);
        inline void RemoveFromQueue(std::vector<GameObject*>& queue,std::string name);

        void InitSprite(SpriteList*sprite);
        void InitPhysicWorld();

        void ResetParam();
        void SetupCamera(Camera* camera);
        void SetupShader(D3DShader* shader,std::unordered_map<std::string,ExtraData>& data);
        void RenderLight();
        void RenderShadow();
        void Render(bool renderUI=false);
        void RenderObject(GameObject* obj);
        void RenderSprite(Sprite* sprite);
        void RenderMesh(Mesh* mesh);
        void RenderModel(ModelMesh* mm);

        void RenderSkybox();

        void UpdateChild(Node* node);
        void FixedUpdateChild(Node*node);
        void AwakeChild(Node* node);
        void DestroyChild(Node*node);
        void PrintChild(int level,Node*node);

        XMMATRIX GetParentModelMatrix(Node* node);


        std::vector<Camera*>mCamera;
        Camera* mShadowCamera;
        Camera* mReflectCamera;
        Node* mRoot;

        int mDirLightCount;
        int mPointLightCount;
        int mSpotLightCount;

        std::vector<BaseLight*> mLight;
        std::vector<GameObject*> mBackGroundQueue;
        std::vector<GameObject*> mAlphaTestQueue;
        std::vector<GameObject*> mTransparentQueue;
        std::vector<GameObject*> mGeometryQueue;
        std::vector<GameObject*> mUIQueue;


        std::set<GameObject*> mRemovedObjects;
    };
}


#endif //GAME_SCENE_H
