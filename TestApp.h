//
// Created by 田淙宇 on 2019/12/2.
//
/*todo:
 * 测试spritelist组件
 *
 * */
#ifndef GAME_TESTAPP_H
#define GAME_TESTAPP_H

#include "App/D3DApp.h"
#include "App/D3DMesh.h"
#include "GameObject/Scene.h"
#include "GameObject/Cube.h"
#include "Component/Transform.h"
#include "App/D3DSourceManager.h"
#include "GameObject/Plane.h"
#include "GameObject/Terrain.h"
#include "Component/Mesh.h"
#include "Component/CameraController.h"
#include "ModelImporter.h"
#include "GameObject/Sphere.h"
#include "GameObject/Water.h"
#include "TestAnimManager.h"
#include "App/AnimManager.h"
#include "Component/Animator.h"
#include "GameObject/PhysicalThing/PhysicalWorld.h"
#include "GameObject/Game/Ball.h"
#include "Component/Game/CharacterController.h"
#include "Component/Game/Enemy.h"

using namespace GRCore;
class TestApp:public D3DApp{
public:
    TestApp(HINSTANCE hinstance):D3DApp(hinstance){

    }

    virtual void OnInitialize(){
        std::cout<<"start init"<<std::endl;
        std::cout<<"default tex transform:"<<DEFAULT_TEX_TRANSFORM<<std::endl;


        mCurScene=new Scene();
        manager=new TestAnimManager();

        PhysicalWorld::Initialize(XMFLOAT3(-50,-50,-50),XMFLOAT3(50,50,50));

        InitObj();


//        importer.LoadAnim("model/yeshou/FBX/Wolf 2.fbx","testAnimation",0);
//
//        AnimManager::GetAnimManager()->LoadAnimationClip("wolf_run","testAnimation",0,30);
//        AnimManager::GetAnimManager()->LoadAnimationClip("wolf_idle","testAnimation",0,30);
//
//        std::cout<<"start load model"<<std::endl;
//        GameObject* obj=importer.LoadModel("model/yeshou/FBX/Wolf 2.fbx","testmodel",mCurScene);
//        mCurScene->PrintObj(obj);
//        obj->GetComponent<Transform>()->SetScale(0.01f,0.01f,0.01f);
//        obj->GetComponent<Transform>()->SetPosition(.0f,3.0f,.0f);


//        std::cout<<"start init animator"<<std::endl;
//        Animator* animator=new Animator();
//        obj->AddComponent(animator);
//        animator->AddAnimation("run","wolf_run",1.0f,true);
//        animator->AddAnimation("idle","wolf_idle",1.0f,true);
//        animator->SetInterval(1);
//        animator->SetCurState("run");



        Terrain* plane=new Terrain();
        plane->SetName("MainPlane");
        Transform *pt=plane->GetComponent<Transform>();
        Mesh* pm=plane->GetComponent<Mesh>();
        pm->SetMaterialName("grass");
        XMFLOAT4X4 t=XMFLOAT4X4();
        XMStoreFloat4x4(&t,XMMatrixScaling(5.0f,5.0f,1.0f));
        pm->SetTexture({"grass_texture",t});
        pt->SetPosition(.0f,.0f,.0f);
        plane->AddComponent(new TerrainCollider());

//        Plane* w=new Plane();
//        w->SetName("MainPlane0");
//        w->SetLayer(2);
//        w->SetExtraData("g_reflection",ExtraData("reflection_texture"));
//        Transform *pw=w->GetComponent<Transform>();
//        pw->SetScale(10.0f,10.0f,10.0f);
//        pw->SetPosition(.0f,.0f,.0f);
//        pm=w->GetComponent<Mesh>();
//        XMStoreFloat4x4(&t,XMMatrixScaling(2.0f,2.0f,1.0f));
//        pm->SetTexture({"water_texture",t});
//        pm->SetMaterialName("water");
//        pm->SetShaderName("WaterShader");

        Water* w=new Water(1000,1000);
        w->SetName("TestWater");
        w->SetReflectCameraName("ReflectCamera");
        w->SetFlowDirection(XMFLOAT2(.02f,.1f));
        w->AddWave({.6f,0.66f,1.0f,0.5f,XMFLOAT2(0.6f,0.24f)});
        //w->AddWave({.4f,0.78f,0.8f,0.9f,XMFLOAT2(0.4f,-0.32f)});




        GameObject* trees=new GameObject();
        trees->SetName("Trees");
        SpriteList* sl=new SpriteList("tree0_texture");
        sl->AddItem(XMFLOAT3(1.0f,1.5f,5.0f),XMFLOAT2(3.0f,2.0f));
        sl->AddItem(XMFLOAT3(2.0f,1.7f,4.0f),XMFLOAT2(3.0f,2.0f));
        trees->AddComponent(sl);

        GameObject* grasses=new GameObject();
        grasses->SetName("Grasses");
        SpriteList* gsl=new SpriteList("grasses_texture");
        gsl->AddItem(XMFLOAT3(0.5f,.5f,3.0f),XMFLOAT2(3.0f,2.0f));
        gsl->AddItem(XMFLOAT3(0.1f,.5f,1.0f),XMFLOAT2(3.0f,2.0f));
        gsl->AddItem(XMFLOAT3(0.1f,.5f,0.3f),XMFLOAT2(3.0f,2.0f));
        gsl->AddItem(XMFLOAT3(0.1f,.5f,0.4f),XMFLOAT2(3.0f,2.0f));
        gsl->AddItem(XMFLOAT3(0.1f,.5f,0.5f),XMFLOAT2(3.0f,2.0f));
        gsl->AddItem(XMFLOAT3(0.1f,.5f,0.6f),XMFLOAT2(3.0f,2.0f));
        gsl->AddItem(XMFLOAT3(0.1f,.5f,1.3f),XMFLOAT2(3.0f,2.0f));
        gsl->AddItem(XMFLOAT3(0.1f,.5f,1.4f),XMFLOAT2(3.0f,2.0f));
        gsl->AddItem(XMFLOAT3(0.1f,.5f,2.3f),XMFLOAT2(3.0f,2.0f));
        gsl->AddItem(XMFLOAT3(-0.5f,.5f,1.3f),XMFLOAT2(3.0f,2.0f));
        gsl->AddItem(XMFLOAT3(-0.5f,.5f,1.4f),XMFLOAT2(3.0f,2.0f));
        gsl->AddItem(XMFLOAT3(-0.5f,.5f,0.3f),XMFLOAT2(3.0f,2.0f));
        gsl->AddItem(XMFLOAT3(-0.5f,.5f,2.0f),XMFLOAT2(3.0f,2.0f));
        grasses->AddComponent(gsl);
        grasses->GetComponent<SpriteList>()->SetShader("grassShader");
        grasses->SetExtraData("g_TexAlpha",ExtraData("grass_alpha"));


        GameObject* sprite=new GameObject();
        sprite->SetName("spriteobj");
        Sprite*sp=new Sprite("lookdown_texture");
        sp->SetShader("Default_sprite_shader");
        sprite->AddComponent(sp);
        (sprite->GetComponent<Transform>())->SetPosition(0.5f,-0.5f,0.0f);
        (sprite->GetComponent<Transform>())->SetScale(0.5f,0.5f,0.0f);




        mCurScene->AddChild(nullptr,plane);
        mCurScene->AddChild(nullptr,w);
        mCurScene->AddChild(nullptr,trees);
        mCurScene->AddChild(nullptr,sprite);
        mCurScene->AddChild(nullptr,grasses);

        mCurScene->SetRenderType(w,RT_Transparent);
        mCurScene->SetRenderType(trees,RT_AlphaTest);
        mCurScene->SetRenderType(sprite,RT_UI);
        mCurScene->SetRenderType(grasses,RT_AlphaTest);

        GameObject* obj=mCurScene->FindGameObjectByName("majorModel");

        mCamera=new Camera();
        mCamera->SetName("MainCamera");
        mCamera->AddComponent(new CameraController(8.0f,obj));
        mCamera->SetPosition(0.0f,5.0f,-10.0f);
        mCurScene->AddCamera(mCamera);

        nCamera=new Camera();
        nCamera->SetName("ReflectCamera");
        nCamera->SetPosition(0.0f,-1.0f,-1.0f);
        nCamera->SetCullingMask(2);
        nCamera->SetRenderTarget("reflection_texture");
        mCurScene->AddCamera(nCamera);

        Camera* lookdown=new Camera();
        lookdown->SetName("LookDownCamera");
        lookdown->SetRenderTarget("lookdown_texture");
        lookdown->SetDirection(XMFLOAT3(.0F,.0F,1.0F),XMFLOAT3(.0F,-1.0F,.0F));
        lookdown->SetPosition(.0f,15.0f,.0f);
        //lookdown->SetProjType(true);
        mCurScene->AddCamera(lookdown);

        //mDirLight=new DirectionLight();
        light=new DirectionLight();
        light->SetDirection(0.0f,-3.0f,-1.0f);
        mCurScene->AddLight(light);
        std::cout<<"initialize finished"<<std::endl;

    }

    virtual void OnUpdate(const GameTimer&gt){
        std::cout<<"delta time:"<<gt.DeltaTime()<<std::endl;
        //manager->UpdateModelMesh(mm,a);


        float rotate=gt.DeltaTime()*0.25*XM_PI;
        rotation=(rotation+rotate);
        if(rotation>2*XM_PI){
            rotation-=2*XM_PI;
        }



//        GameObject* wave=mCurScene->FindGameObjectByName("MainPlane0");
//        wave->SetExtraData("ReflectView",ExtraData(nCamera->GetViewMatrix()));
//        XMFLOAT4X4 matTrans=wave->GetComponent<Mesh>()->GetModelTexTransform();
//        float tu=matTrans(3,0);
//        float tv=matTrans(3,1);
//
        SetReflectCamera();
        SetLookdownCamera();

//
//        tu+=0.02f*gt.DeltaTime();
//        tv+=0.1f*gt.DeltaTime();
//
//        if(tu>1.0f){
//            tu-=1.0f;
//        }
//        if(tv>1.0f){
//            tv-=1.0f;
//        }
//        matTrans(3,0)=tu;
//        matTrans(3,1)=tv;
//        wave->GetComponent<Mesh>()->SetModelTexTransform(matTrans);
        PhysicalWorld::GetPhysicalWorld()->update(gt.DeltaTime());

    }

private:
    int count=0;
    float lastTime=0;

    void InitObj(){
        D3DSourceManager::GetSourceManager()->LoadEffect("WaterShader","Shader/Water.fx",DEFAULT_LAYOUT,DEFAULT_LAYOUT_SIZE);
        D3DSourceManager::GetSourceManager()->LoadEffect("grassShader","Shader/Grass.fx",DEFAULT_SPRITE_LAYOUT,DEFAULT_SPRITE_LAYOUT_SIZE);

        D3DSourceManager::GetSourceManager()->LoadTexture("test_texture","Texture/container.jpg");
        D3DSourceManager::GetSourceManager()->LoadTexture("water_texture","Texture/water.dds");
        D3DSourceManager::GetSourceManager()->LoadTexture("grass_texture","Texture/grass.dds");
        D3DSourceManager::GetSourceManager()->LoadTexture("fence_texture","Texture/WireFence.dds");
        D3DSourceManager::GetSourceManager()->LoadTexture("tree0_texture","Texture/tree3.dds");
        D3DSourceManager::GetSourceManager()->LoadTexture("DefaultSkyBox","Texture/sunsetCube.dds");
        D3DSourceManager::GetSourceManager()->LoadTexture("grasses_texture","Texture/grass.png");
        D3DSourceManager::GetSourceManager()->LoadTexture("grass_alpha","Texture/grassAlpha.jpeg");

        D3DSourceManager::GetSourceManager()->LoadRenderTarget(DEFAULT_SHADOW_TEXTURE0_NAME,nullptr,2048,2048);
        D3DSourceManager::GetSourceManager()->LoadRenderTarget(DEFAULT_SHADOW_TEXTURE1_NAME,nullptr);
        D3DSourceManager::GetSourceManager()->LoadRenderTarget(DEFAULT_SHADOW_TEXTURE2_NAME,nullptr);
        D3DSourceManager::GetSourceManager()->LoadRenderTarget(DEFAULT_SHADOW_TEXTURE3_NAME,nullptr);

        GR_SKYBOX_TEXTURE="DefaultSkyBox";

        D3DSourceManager::GetSourceManager()->LoadRenderTarget("reflection_texture",nullptr);
        D3DSourceManager::GetSourceManager()->LoadRenderTarget("lookdown_texture",nullptr);
        ModelImporter importer;


        MATERIAL_VALUE grass{
                XMFLOAT4(0.2F,0.6F,0.2F,1.0F),
                XMFLOAT4(0.0f,.0f,.0f,1.0f),
                XMFLOAT3(0.01f,0.01f,0.01f),
                0.125f,
                0
        };
        MATERIAL_VALUE water{
                XMFLOAT4(0.0F,0.2F,0.6F,.6F),
                XMFLOAT4(.5f,.5f,.5f,1.0f),
                XMFLOAT3(.1F,.1F,.1F),
                1.F,
                1
        };
        MATERIAL_VALUE green{
                XMFLOAT4(0.F,1.0F,0.0F,1.0F),
                XMFLOAT4(1.0f,1.0f,1.0f,1.0f),
                XMFLOAT3(1.5f,1.5f,1.5f),
                1.0f,
                1
        };
        MATERIAL_VALUE fire{
                XMFLOAT4(1.0f,.0f,.0f,1.0f),
                XMFLOAT4(1.0f,1.0f,1.0f,1.0f),
                XMFLOAT3(1.5f,1.5f,1.5f),
                1.0f,
                1
        };

        D3DSourceManager::GetSourceManager()->LoadMaterial("water",water);
        D3DSourceManager::GetSourceManager()->LoadMaterial("grass",grass);
        D3DSourceManager::GetSourceManager()->LoadMaterial("green",green);
        D3DSourceManager::GetSourceManager()->LoadMaterial("fire",fire);
        // 加载camel动画及模型
        // camel
        importer.LoadAnim("model/yeshou/FBX/Camel.fbx", "camelAnimation", 0);
        AnimManager::GetAnimManager()->LoadAnimationClip("camel_run", "camelAnimation", 112, 151-112);
        AnimManager::GetAnimManager()->LoadAnimationClip("camel_idle", "camelAnimation", 0, 61-0);
        AnimManager::GetAnimManager()->LoadAnimationClip("camel_walk", "camelAnimation", 62, 111 - 62);
        AnimManager::GetAnimManager()->LoadAnimationClip("camel_attack", "camelAnimation", 441, 483 - 441);
        AnimManager::GetAnimManager()->LoadAnimationClip("camel_die", "camelAnimation", 332, 379 - 332);
        AnimManager::GetAnimManager()->LoadAnimationClip("camel_jump", "camelAnimation", 239, 290 - 239);
        GameObject* objCamel = importer.LoadModel("model/yeshou/FBX/Camel.fbx", "camelModel", mCurScene);
        objCamel->GetComponent<Transform>()->SetScale(0.01f, 0.01f, 0.01f);
        objCamel->GetComponent<Transform>()->SetPosition(-5.0f, 2.0f, -2.0f);
        Animator* animator = new Animator();
        objCamel->AddComponent(animator);
        animator->AddAnimation("run", "camel_run", 1.0f, true);
        animator->AddAnimation("idle", "camel_idle", 1.0f, true);
        animator->AddAnimation("jump", "camel_jump", 1.0f, true);
        animator->AddAnimation("die", "camel_die", 1.0f, true);
        animator->AddAnimation("attack", "camel_attack", 1.0f, true);
        animator->AddAnimation("walk", "camel_walk", 1.0f, true);
        animator->SetInterval(1);
        animator->SetCurState("run");
        objCamel->AddComponent(new RectCollider(false,.0f,1.5f,.0f,XMFLOAT3(.5f,1.5f,1.3f),false,true));
        objCamel->AddComponent(new Enemy(3));


        // puma
        importer.LoadAnim("model/yeshou/FBX/Puma.fbx", "pumaAnimation", 0);
        AnimManager::GetAnimManager()->LoadAnimationClip("puma_run", "pumaAnimation", 256, 294 - 256);
        AnimManager::GetAnimManager()->LoadAnimationClip("puma_idle", "pumaAnimation", 0, 82 - 0);
        AnimManager::GetAnimManager()->LoadAnimationClip("puma_jump", "pumaAnimation", 378, 419 - 378);
        AnimManager::GetAnimManager()->LoadAnimationClip("puma_attack", "pumaAnimation", 532, 573 - 532);
        AnimManager::GetAnimManager()->LoadAnimationClip("puma_die", "pumaAnimation", 480, 529 - 480);
        AnimManager::GetAnimManager()->LoadAnimationClip("puma_walk", "pumaAnimation", 207, 255 - 207);
        GameObject* objPuma = importer.LoadModel("model/yeshou/FBX/Puma.fbx", "pumaModel", mCurScene);
        objPuma->GetComponent<Transform>()->SetScale(0.01f, 0.01f, 0.01f);
        objPuma->GetComponent<Transform>()->SetPosition(-2.5f, 2.0f, .0f);
        animator = new Animator();
        objPuma->AddComponent(animator);
        animator->AddAnimation("run", "puma_run", 1.0f, true);
        animator->AddAnimation("idle", "puma_idle", 1.0f, true);
        animator->AddAnimation("jump", "puma_jump", 1.0f, true);
        animator->AddAnimation("die", "puma_die", 1.0f, false);
        animator->AddAnimation("attack", "puma_attack", 1.0f, true);
        animator->AddAnimation("walk", "puma_walk", 1.0f, true);
        animator->SetInterval(1);
        animator->SetCurState("walk");
        objPuma->AddComponent(new RectCollider(false,.0f,.6f,.0f,XMFLOAT3(.5f,.6f,.8f),false,true));
        objPuma->AddComponent(new Enemy(2));



        // major model
        importer.LoadAnim("model/yeshou/FBX/Wolf 2.fbx","testAnimation",0);

        AnimManager::GetAnimManager()->LoadAnimationClip("wolf_run","testAnimation",359,20);
        AnimManager::GetAnimManager()->LoadAnimationClip("wolf_idle","testAnimation",0,82);
        AnimManager::GetAnimManager()->LoadAnimationClip("wolf_walk","testAnimation",318,40);
        AnimManager::GetAnimManager()->LoadAnimationClip("wolf_attack","testAnimation",83,154-83);

        std::cout<<"start load model"<<std::endl;
        GameObject* obj=importer.LoadModel("model/yeshou/FBX/Wolf 2.fbx","majorModel",mCurScene);
        mCurScene->PrintObj(obj);
        obj->GetComponent<Transform>()->SetScale(0.01f,0.01f,0.01f);
        obj->GetComponent<Transform>()->SetPosition(-5.0f,2.0f,.0f);

        std::cout<<"start init animator"<<std::endl;
        animator=new Animator();
        obj->AddComponent(animator);
        animator->AddAnimation("walk","wolf_walk",1.0f,true);
        animator->AddAnimation("run","wolf_run",1.0f,true);
        animator->AddAnimation("idle","wolf_idle",1.0f,true);
        animator->AddAnimation("attack","wolf_attack",1.0f,true);
        animator->SetInterval(1);
        animator->SetCurState("idle");

        obj->AddComponent(new CharacterController(3.0f));
        obj->AddComponent(new RectCollider(false,.0f,.6f,.0f,XMFLOAT3(.5f,.6f,.8f),false,true));

    }
    void SetLookdownCamera(){
        GameObject* obj=mCurScene->FindGameObjectByName("majorModel");
        XMFLOAT3 pos=obj->GetComponent<Transform>()->GetPosition();
        Camera* mapCamera=mCurScene->FindCameraByName("LookDownCamera");
        XMFLOAT3 cameraPos=mapCamera->GetComponent<Transform>()->GetPosition();
        mapCamera->SetPosition(pos.x,cameraPos.y,pos.z);
    }
    void SetReflectCamera(){
        Transform* t=mCamera->GetComponent<Transform>();
        XMFLOAT3 pos=t->GetPosition();
        pos.y=-pos.y;
        XMFLOAT3 n=XMFLOAT3(.0f,1.0f,.0f);
        XMVECTOR normal=XMLoadFloat3(&n);

        XMVECTOR up,right,lookat;
        up=XMLoadFloat3(&(mCamera->GetUpDirection()));
        right=XMLoadFloat3(&(mCamera->GetRightDirection()));
        lookat=XMLoadFloat3(&(mCamera->GetLookAtDirection()));




        up=XMVector3Reflect(up,normal);
        right=XMVector3Reflect(right,normal);
        lookat=XMVector3Reflect(lookat,normal);

        std::cout<<"up:"<<up<<" right:"<<right<<" lookat:"<<lookat<<std::endl;
        XMFLOAT3 u,r,l;
        XMStoreFloat3(&u,up);
        XMStoreFloat3(&r,right);
        XMStoreFloat3(&l,lookat);

        nCamera->SetPosition(pos.x,pos.y,pos.z);
        nCamera->SetDirection(u,l);



    }
    TestAnimManager* manager;
    float rotation=0;
    Camera* mCamera;
    Camera* nCamera;
    DirectionLight *light;
};

#endif //GAME_TESTAPP_H
