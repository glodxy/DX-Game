//
// Created by 田淙宇 on 2019/12/11.
//

#include "Scene.h"
#include "../Component/Transform.h"
#include "../App/D3DSourceManager.h"
#include "../App/D3DSpriteList.h"
#include "../App/D3DRenderTarget.h"
#include "../App/D3DSprite.h"
#include "../Component/ModelMesh.h"
#include "PhysicalThing/PhysicalWorld.h"


using namespace GRCore;
using std::string;

void GRCore::DestroyObject(GRCore::Scene *scene, GRCore::GameObject *obj) {
    Collider* collider=nullptr;
    if((collider=obj->GetComponent<SphereCollider>())!=nullptr||(collider=obj->GetComponent<RectCollider>())!=nullptr) {
        std::cout<<"obj collider destroy called"<<std::endl;
        PhysicalWorld::GetPhysicalWorld()->deleteObject(collider);
    }
    scene->DestroyObject(obj);
}

Scene::Scene() {
    mRoot=new Node();
    mRoot->parent=nullptr;
    mRoot->obj=nullptr;
    mDirLightCount=mPointLightCount=mSpotLightCount=0;
}

void Scene::Awake() {
    AwakeChild(mRoot);
    mShadowCamera=new Camera();
}

void Scene::Destroy() {
    DestroyChild(mRoot);
    ReleaseNode(mRoot);
}

void Scene::Render(bool renderUI) {
    GR_CURRENT_MODE=GR_DEFAULT_MODE;
    //渲染背景
    for(auto iter:mBackGroundQueue){
        RenderObject(iter);
    }

    //渲染普通物体
    for(auto iter:mGeometryQueue){
        RenderObject(iter);
    }

    //渲染透明
    GR_CURRENT_MODE=GR_ALPHA_TEST_MODE;
    for(auto iter:mAlphaTestQueue){
        RenderObject(iter);
    }
//    float bf[]={.0f,.0f,.0f,.0f};
//    GR_CONTEXT->OMSetBlendState(GR_BLEND_STATE,bf,0xffffffff);

    //渲染半透明
    GR_CURRENT_MODE=GR_TRANSPARENT_MODE;
    for(auto iter:mTransparentQueue){
        std::cout<<"transparent name:"<<iter->GetName()<<std::endl;
        RenderObject(iter);
    }

    GR_CURRENT_MODE=GR_DEFAULT_MODE;
    RenderSkybox();

    if(renderUI) {
        GR_CURRENT_MODE = GR_UI_MODE;
        for (auto iter:mUIQueue) {
            std::cout << "transparent name:" << iter->GetName() << std::endl;
            RenderObject(iter);
        }
    }

    GR_CURRENT_MODE=GR_DEFAULT_MODE;

//    GR_CONTEXT->OMSetBlendState(0,bf,0xffffffff);
}

void Scene::RenderShadow() {
    GR_CURRENT_MODE=GR_SHADOW_MODE;
    for(int i=0;i<mDirLightCount+mSpotLightCount+mPointLightCount&&i<4;++i) {
        LIGHT l=mLight[i]->GetValue();
        std::string shadowmap=DEFAULT_SHADOW_TEXTURE_NAME+std::to_string(i);
        std::cout<<"start render shadow of light:"<<i<<" to "<<shadowmap<<std::endl;
        mShadowCamera->SetRenderTarget(shadowmap);
        if(i<mDirLightCount){
            XMFLOAT3 up=XMFLOAT3(.0f,1.0f,.0f);
            if(l.direction.x==l.direction.z&&l.direction.x==.0f){
                up=XMFLOAT3(.0f,.0f,1.0f);
            }
            mShadowCamera->SetDirection(up,l.direction);
            float z=l.direction.z;
            if(abs(l.direction.x)>abs(z)){
                z=l.direction.x;
            }
            float distance=(abs(z)/(sqrt(z*z+l.direction.y*l.direction.y)))*75.0f;
            distance+=10.0f;
            mShadowCamera->SetPosition(-l.direction.x*distance,-l.direction.y*distance,-l.direction.z*distance);
            mShadowCamera->SetFOV(XM_PI/2);
            mShadowCamera->SetProjType(true);
        }
        else if(i<mDirLightCount+mPointLightCount){
            mShadowCamera->SetPosition(l.position.x,l.position.y,l.position.z);
            mShadowCamera->SetDirection(XMFLOAT3(.0,.0,1.0),XMFLOAT3(.0f,-1.0f,.0f));
            mShadowCamera->SetFOV(XM_PI);
            mShadowCamera->SetProjType(false);
        }
        else{
            mShadowCamera->SetPosition(l.position.x,l.position.y,l.position.z);
            mShadowCamera->SetDirection(XMFLOAT3(.0f,1.0f,.0f),l.direction);
            mShadowCamera->SetFOV(XM_PI/2);
            mShadowCamera->SetProjType(false);
        }
        SetupCamera(mShadowCamera);
        GR_LIGHT_VIEWPROJ_MATRIX[i]=mShadowCamera->GetViewProjMatrix();
        //渲染背景
        for (auto iter:mBackGroundQueue) {
            RenderObject(iter);
        }

        //渲染普通物体
        for (auto iter:mGeometryQueue) {
            RenderObject(iter);
        }
    }
    GR_CURRENT_MODE=GR_DEFAULT_MODE;
    std::cout<<"render shadow finished"<<std::endl;
}

void Scene::Update() {
    std::cout<<"scene update start"<<std::endl;
    RenderLight();
    ResetParam();
    RenderShadow();
    std::cout << "light setup finished" << std::endl;
    for(int i=0;i<mCamera.size();++i) {
        ResetParam();
        SetupCamera(mCamera[i]);
        bool ui=(mCamera[i]->GetRenderTarget()==DEFAULT_RENDERTARGET_NAME);
        Render(ui);
    }
    std::cout<<"render finished"<<std::endl;
    std::cout<<"update objects"<<std::endl;
    PrintChild(0,mRoot);
    std::cout<<"update objects print finished"<<std::endl;
    UpdateChild(mRoot);
    std::cout<<"start detroy object"<<std::endl;
    DestroyRemovedObject();
    std::cout<<"scene update finished"<<std::endl;
}

void Scene::FixedUpdate() {
    FixedUpdateChild(mRoot);
}

void Scene::AddLight(GRCore::BaseLight *light) {
    switch(light->GetLightType()){
        case DIR_LIGHT_TYPE:{
            mLight.insert(mLight.begin()+mDirLightCount,light);
            mDirLightCount++;
            break;
        }
        case POINT_LIGHT_TYPE:{
            mLight.insert(mLight.begin()+mDirLightCount+mPointLightCount,light);
            mPointLightCount++;
            break;
        }
        case SPOT_LIGHT_TYPE:{
            mLight.insert(mLight.begin()+mDirLightCount+mPointLightCount+mSpotLightCount,light);
            mSpotLightCount++;
            break;
        }
    }
}

void Scene::AddChild(GRCore::Node *root, GRCore::GameObject *obj) {
    Node *child=new Node();
    child->parent=root;
    child->obj=obj;
    obj->SetScene(this);
    obj->SetNode(child);
    SetRenderType(obj,obj->GetRenderType());
    if(root==nullptr){
        child->parent=mRoot;
        mRoot->children.push_back(child);
    }else {
        root->children.push_back(child);
    }

    if((obj->GetComponent<SpriteList>())!=nullptr){
        InitSprite(obj->GetComponent<SpriteList>());
    }
}

void Scene::RemoveChildByName(GRCore::Node *root, std::string name) {
    for(auto iter=root->children.begin();iter!=root->children.end();++iter){
        if((*iter)->obj->GetName()==name){
            Node *n=(*iter);
            ReleaseNode(n);
            root->children.erase(iter);
            break;
        }
    }
}

GameObject* Scene::FindChildByName(GRCore::Node *root, std::string name) {
    if(root->children.empty()){
        return nullptr;
    }
    for(auto iter:root->children){
        if(iter->obj->GetName()==name){
            return iter->obj;
        }
    }
    return nullptr;
}

GameObject* Scene::FindGameObjectByName(std::string name) {
    return SearchByName(mRoot,name);
}

Camera* Scene::FindCameraByName(std::string name) {
    for(auto iter:mCamera){
        if(iter->GetName()==name){
            return iter;
        }
    }
    return nullptr;
}

void Scene::SetParent(GRCore::Node *parent, GRCore::Node *child) {
    std::string name=child->obj->GetName();
    for(auto iter=child->parent->children.begin();iter!=child->parent->children.end();++iter){
        if((*iter)->obj->GetName()==name){
            child->parent->children.erase(iter);
            break;
        }
    }
    child->parent=parent;
    parent->children.push_back(child);

}

void Scene::SetRenderType(GRCore::GameObject *obj, GRCore::RenderType type) {
    RemoveFromRenderQueue(obj);
    switch(type){
        case RT_BackGround:{
            std::cout<<"add back:"<<obj->GetName()<<std::endl;
            mBackGroundQueue.push_back(obj);
            break;
        }
        case RT_Geometry:{
            std::cout<<"add geometry:"<<obj->GetName()<<std::endl;
            mGeometryQueue.push_back(obj);
            break;
        }
        case RT_AlphaTest:{
            mAlphaTestQueue.push_back(obj);
            break;
        }
        case RT_Transparent:{
            std::cout<<"add transparent:"<<obj->GetName()<<std::endl;
            mTransparentQueue.push_back(obj);
            break;
        }
        case RT_UI:{
            mUIQueue.push_back(obj);
            break;
        }
    }
}


GameObject* Scene::SearchByName(GRCore::Node *root, std::string name) {

    if(root->obj!=nullptr&&root->obj->GetName()==name){
        return root->obj;
    }
    if(root->children.empty()){
        return nullptr;
    }
    GameObject* result=nullptr;
    for(auto iter:root->children){
        if((result=SearchByName(iter,name))!=nullptr){
            return result;
        }
    }
    return nullptr;
}
void Scene::ReleaseNode(GRCore::Node *node) {
    std::cout<<"relese node"<<std::endl;
    if(node->parent!=nullptr){
        for(auto iter=node->parent->children.begin();iter!=node->parent->children.end();){
            if((*iter)==node){
                std::string parentName="root";
                if(node->parent->obj!=nullptr){
                    parentName=node->parent->obj->GetName();
                }
                std::cout<<"erase child:"<<node->obj->GetName()<<" from paren:"<<parentName<<std::endl;
                node->parent->children.erase(iter);
                break;
            }
            else{
                ++iter;
            }
        }
        node->parent=nullptr;
    }
    std::cout<<"remove from parent"<<std::endl;
    if(!node->children.empty()){
        std::vector<Node*> children=node->children;
        for(auto iter:children){
            ReleaseNode(iter);
        }
    }
    delete node->obj;
    node->obj=nullptr;
    delete node;
}

void Scene::RemoveFromRenderQueue(GRCore::GameObject *obj) {

    switch(obj->GetRenderType()){
        case RT_BackGround:{
            RemoveFromQueue(mBackGroundQueue,obj->GetName());
            break;
        }
        case RT_Geometry:{
            std::cout<<"remove obj from geometry"<<obj->GetName()<<std::endl;
            RemoveFromQueue(mGeometryQueue,obj->GetName());
            break;
        }
        case RT_AlphaTest:{
            RemoveFromQueue(mAlphaTestQueue,obj->GetName());
            break;
        }
        case RT_Transparent:{
            RemoveFromQueue(mTransparentQueue,obj->GetName());
            break;
        }
        case RT_UI:{
            RemoveFromQueue(mUIQueue,obj->GetName());
        }
    }
    std::cout<<"remove "<<obj->GetName()<<" finished"<<std::endl;
}

void Scene::RemoveFromQueue(std::vector<GameObject *> &queue, std::string name) {
    for(auto iter=queue.begin();iter!=queue.end();++iter){
        if((*iter)->GetName()==name){
            queue.erase(iter);
            break;
        }
    }
}

void Scene::UpdateChild(GRCore::Node *node) {
    if(node->obj!=nullptr) {
        node->obj->update();
    }
    if(node->children.empty()){
        return;
    }
    std::vector<Node*> children=node->children;
    for(auto iter:children){
        UpdateChild(iter);
    }
}

void Scene::FixedUpdateChild(GRCore::Node *node) {
    if(node->obj!=nullptr) {
        node->obj->fixedUpdate();
    }
    if(node->children.empty()){
        return;
    }
    std::vector<Node*> children=node->children;
    for(auto iter:children){
        FixedUpdateChild(iter);
    }
}

void Scene::AwakeChild(GRCore::Node *node) {
    if(node->obj!=nullptr) {
        node->obj->awake();
    }
    if(node->children.empty()){
        return;
    }
    std::vector<Node*> children=node->children;
    for(auto iter:children){
        AwakeChild(iter);
    }
}

void Scene::DestroyChild(GRCore::Node *node) {
    if(node->obj!=nullptr) {
        node->obj->destroy();
    }
    if(node->children.empty()){
        return;
    }
    std::vector<Node*> children=node->children;
    for(auto iter:children){
        DestroyChild(iter);
    }
}

void Scene::RenderLight() {
    //设置方向光
    GR_DIRLIGHT_SIZE=mDirLightCount;
    if(mDirLightCount>0){
        for(int i=0;i<GR_DIRLIGHT_SIZE;++i){
            GR_LIGHTS[i]=mLight[i]->GetValue();
        }
    }
    std::cout<<"dir light set finished"<<std::endl;
    //设置点光源
    GR_POINTLIGHT_SIZE=mPointLightCount;
    if(mPointLightCount>0){
        for(int i=GR_DIRLIGHT_SIZE;i<GR_DIRLIGHT_SIZE+GR_POINTLIGHT_SIZE;++i){
            GR_LIGHTS[i]=mLight[i]->GetValue();
        }
    }
    std::cout<<"point light set finished"<<std::endl;
    //设置聚光光源
    GR_SPOTLIGHT_SIZE=mSpotLightCount;
    if(mSpotLightCount>0){
        for(int i=GR_DIRLIGHT_SIZE+GR_POINTLIGHT_SIZE;i<GR_DIRLIGHT_SIZE+GR_POINTLIGHT_SIZE+GR_SPOTLIGHT_SIZE;++i){
            GR_LIGHTS[i]=mLight[i]->GetValue();
        }
    }
    //RenderShadow();
}

void Scene::SetupCamera(Camera* camera) {
    GR_VIEW_MATRIX=camera->GetViewMatrix();
    if(camera->IsOrtho()){
        GR_PROJ_MATRIX=camera->GetOrthoMatrix();
    }
    else {
        GR_PROJ_MATRIX = camera->GetProjMatrix();
    }
    std::cout<<"view matrix:"<<GR_VIEW_MATRIX<<std::endl;
    std::cout<<"proj matrix:"<<GR_PROJ_MATRIX<<std::endl;
    auto t=camera->GetComponent<Transform>();
    GR_CAMERA_POS=t->GetPosition();

    D3DRenderTarget* prtv=(D3DRenderTarget*)(D3DSourceManager::GetSourceManager()->GetSource(camera->GetRenderTarget()).source);
    prtv->Clear();
    GR_CONTEXT->OMSetRenderTargets(1,prtv->GetRTV().GetAddressOf(),prtv->GteDSV().Get());
    if(camera->GetRenderTarget()==DEFAULT_RENDERTARGET_NAME){
        GR_CURRENT_VIEWPORT.Width=GR_WIDTH;
        GR_CURRENT_VIEWPORT.Height=GR_HEIGHT;
    }else {
        GR_CURRENT_VIEWPORT.Width = prtv->GetWidth();
        GR_CURRENT_VIEWPORT.Height = prtv->GetHeight();
    }
    std::cout<<"current target:"<<camera->GetRenderTarget()<<", viewport:"<<GR_CURRENT_VIEWPORT.Width<<","<<GR_CURRENT_VIEWPORT.Height<<std::endl;
    GR_CONTEXT->RSSetViewports(1,&GR_CURRENT_VIEWPORT);
    GR_CULLING_MASK=camera->GetCullingMask();
    camera->update();
}

void Scene::RenderObject(GRCore::GameObject *obj) {
    if(((obj->GetLayerMask())&GR_CULLING_MASK)==0) {
        std::cout << "render obj:" << obj->GetName() << std::endl;
        Mesh *mesh = nullptr;
        SpriteList *sp = nullptr;
        Sprite *p = nullptr;
        ModelMesh* mm=nullptr;
        if((mm=obj->GetComponent<ModelMesh>())!=nullptr){
            std::cout<<"render model of"<<obj->GetName()<<std::endl;
            RenderModel(mm);
        }else if ((mesh = obj->GetComponent<Mesh>()) != nullptr) {
            std::cout << "render mesh:" << obj->GetName() << std::endl;
            RenderMesh(mesh);
        } else if ((sp = obj->GetComponent<SpriteList>()) != nullptr) {
            std::unordered_map<std::string,ExtraData> extraData=obj->GetExtraDatas();
            Source s=D3DSourceManager::GetSourceManager()->GetSource(sp->GetShaderName());
            D3DShader* shader=(D3DShader*)s.source;
            SetupShader(shader,extraData);
            ((D3DSpriteList *) D3DSourceManager::GetSourceManager()->GetSource(obj->GetName()).source)->Render();
        } else if ((p = obj->GetComponent<Sprite>()) != nullptr) {
            RenderSprite(p);
        }
    }
}

void Scene::RenderModel(GRCore::ModelMesh *mm) {
    std::cout<<"start render model:"<<mm->GetGameObject()->GetName()<<std::endl;
    std::vector<Mesh> meshes = mm->GetMeshes();
    std::vector<XMFLOAT4X4> bones;
    bones.resize(MAX_BONES,DEFAULT_ZERO_MATRIX);
    std::cout<<"bones:"<<mm->GetBoneSize()<<std::endl;
    if(mm->GetBoneSize()>0) {
        std::vector<XMFLOAT4X4> b= mm->GetBones();
        std::cout<<"get bones size:"<<b.size()<<std::endl;
        for(int i=0;i<b.size()&&i<MAX_BONES;++i){
            bones[i]=b[i];
        }
    }
    GR_BONES_MATRIX = &bones[0];
    std::cout<<"start render each mesh:"<<meshes.size()<<std::endl;
    for(auto iter:meshes){
        std::cout<<"start render mesh name:"<<iter.GetGameObject()->GetName()<<std::endl;
        RenderMesh(&iter);
    }

    GR_BONES_MATRIX=nullptr;
}

void Scene::RenderMesh(GRCore::Mesh *mesh) {
    std::cout<<"start render mesh:"<<mesh->GetGameObject()->GetName()<<std::endl;
    GameObject* object=mesh->GetGameObject();
    std::unordered_map<std::string,ExtraData> extraData=object->GetExtraDatas();
    Transform* trans=object->GetComponent<Transform>();
    if(trans== nullptr){
        std::cerr<<"error get component transform"<<std::endl;
    }
    XMMATRIX modelMatrix=GetParentModelMatrix(object->GetNode());
    XMStoreFloat4x4(&GR_MODEL_MATRIX,modelMatrix);
    std::cout<<"set model matrix finished"<<std::endl;

    XMMATRIX mmatrix=XMLoadFloat4x4(&GR_MODEL_MATRIX);
    mmatrix=XMMatrixInverse(nullptr,mmatrix);
    mmatrix=XMMatrixTranspose(mmatrix);
    XMStoreFloat4x4(&GR_NORMAL_WORLD_MATRIX,mmatrix);

    GR_MODEL_TEX_TRANSFORM_MATRIX=mesh->GetModelTexTransform();

    std::vector<ID3D11ShaderResourceView*> texArray;
    std::vector<XMFLOAT4X4> texTransArray;
    Source m=D3DSourceManager::GetSourceManager()->GetSource(mesh->GetMeshName());
    Source s=D3DSourceManager::GetSourceManager()->GetSource(mesh->GetShaderName());
    Source ma=D3DSourceManager::GetSourceManager()->GetSource(mesh->GetMaterialName());

    std::vector<TextureData> texs=mesh->GetTextures();
    for(auto iter:texs){
        Source tex=D3DSourceManager::GetSourceManager()->GetSource(iter.tex);
        ID3D11ShaderResourceView *texture;
        if(tex.type==GR_TEXTURE) {
            texture = ((D3DTexture *) tex.source)->GetTexture();
        }else if(tex.type==GR_RENDERTARGET){
            texture=((D3DRenderTarget*)tex.source)->GetResource().Get();
        }
        texArray.push_back(texture);
        texTransArray.push_back(iter.texTrans);
    }

    D3DTexture* skybox=(D3DTexture*)D3DSourceManager::GetSourceManager()->GetSource(GR_SKYBOX_TEXTURE).source;

    D3DMesh *dm=(D3DMesh*)m.source;
    D3DShader* shader=(D3DShader*)s.source;

    SetupShader(shader,extraData);

    MATERIAL_VALUE* material=(MATERIAL_VALUE*)ma.source;
    std::cout<<"tex of "<<object->GetName()<<texs.size()<<std::endl;
    if(GR_BONES_MATRIX==nullptr) {
        dm->Render(shader, texArray, texTransArray, material, skybox->GetTexture());
    }else{
        std::cout<<"render model:"<<object->GetName()<<std::endl;
        dm->Render(shader,texArray,texTransArray,material,GR_BONES_MATRIX,true,skybox->GetTexture());
    }
}

void Scene::RenderSprite(GRCore::Sprite *sprite) {
    GameObject* object=sprite->GetGameObject();
    std::unordered_map<std::string,ExtraData> extraData=object->GetExtraDatas();
    Transform* trans=object->GetComponent<Transform>();
    if(trans== nullptr){
        std::cerr<<"error get component transform"<<std::endl;
    }
    GR_MODEL_MATRIX=trans->GetMatrix();
    std::cout<<object->GetName()<<"  model matrix:"<<GR_MODEL_MATRIX<<std::endl;
    XMMATRIX mmatrix=XMLoadFloat4x4(&GR_MODEL_MATRIX);
    mmatrix=XMMatrixInverse(nullptr,mmatrix);
    mmatrix=XMMatrixTranspose(mmatrix);
    XMStoreFloat4x4(&GR_NORMAL_WORLD_MATRIX,mmatrix);

    GR_MODEL_TEX_TRANSFORM_MATRIX=sprite->GetTextureTranform();



    Source s=D3DSourceManager::GetSourceManager()->GetSource(sprite->GetShaderName());
    Source psp=D3DSourceManager::GetSourceManager()->GetSource(DEFAULT_SPRITE_NAME);

    Source tex=D3DSourceManager::GetSourceManager()->GetSource(sprite->GetTextureName());
    ID3D11ShaderResourceView *texture;
    if(tex.type==GR_TEXTURE) {
        texture = ((D3DTexture *) tex.source)->GetTexture();
    }else if(tex.type==GR_RENDERTARGET){
        texture=((D3DRenderTarget*)tex.source)->GetResource().Get();
    }


    D3DSprite* p=(D3DSprite*)psp.source;
    D3DShader* shader=(D3DShader*)s.source;
    SetupShader(shader,extraData);
    std::cout<<"setup shader finished:"<<object->GetName()<<std::endl;

    p->Render(shader,texture,sprite->GetTextureTranform());
}

void Scene::RenderSkybox() {
    Source sph=D3DSourceManager::GetSourceManager()->GetSource(DEFAULT_SKYBOX_MESH_NAME);
    Source sh=D3DSourceManager::GetSourceManager()->GetSource(DEFAULT_SKYBOX_SHADER_NAME);
    Source t=D3DSourceManager::GetSourceManager()->GetSource(GR_SKYBOX_TEXTURE);


    D3DMesh* mesh=(D3DMesh*)sph.source;
    D3DShader* shader=(D3DShader*)sh.source;
    D3DTexture* tex=(D3DTexture*)t.source;

    GR_MODEL_MATRIX=GR_NORMAL_WORLD_MATRIX=DEFAULT_TEX_TRANSFORM;
    GR_MODEL_TEX_TRANSFORM_MATRIX=DEFAULT_TEX_TRANSFORM;
    MATERIAL_VALUE* material=(MATERIAL_VALUE*)(D3DSourceManager::GetSourceManager()->GetSource(DEFAULT_MATERIAL_NAME).source);


    std::vector<ID3D11ShaderResourceView*> texs;
    std::vector<XMFLOAT4X4> trans;
    mesh->Render(shader,texs,trans,material,tex->GetTexture());
}

void Scene::SetupShader(GRCore::D3DShader *shader, std::unordered_map<std::string,ExtraData> &data) {
    std::vector<ID3D11ShaderResourceView*> shadowMaps;
    for(int i=0;i<4;++i){
        D3DRenderTarget*ps=(D3DRenderTarget*)(D3DSourceManager::GetSourceManager()->GetSource(DEFAULT_SHADOW_TEXTURE_NAME+std::to_string(i))).source;
        shadowMaps.push_back(ps->GetResource().Get());
    }
    shader->SetTextureArray("g_shadowMap",&shadowMaps[0],4);
    shader->SetMatrix4Array("g_lightMatrix",GR_LIGHT_VIEWPROJ_MATRIX,4);
    for(auto d:data){
        switch(d.second.dataType){
            case GR_DATATYPE_FLOAT3:{
                XMFLOAT3* pd= reinterpret_cast<XMFLOAT3*>(d.second.data);
                shader->SetVector(d.first,*pd);
                break;
            }
            case GR_DATATYPE_FLOAT2:{
                XMFLOAT2* pd= reinterpret_cast<XMFLOAT2*>(d.second.data);
                shader->SetVector(d.first,*pd);
                break;
            }
            case GR_DATATYPE_FLOAT:{

            }
            case GR_DATATYPE_INT:{
                int* pi= reinterpret_cast<int*>(d.second.data);
                shader->SetInt(d.first,*pi);
                break;
            }
            case GR_DATATYPE_TEXTURE:{
                char *texname=reinterpret_cast<char*>(d.second.data);
                std::string tempName(texname,texname+strlen(texname));
                Source s=D3DSourceManager::GetSourceManager()->GetSource(tempName);
                if(s.type==GR_TEXTURE) {
                    D3DTexture *tex = (D3DTexture *)s.source;
                    shader->SetTexture(d.first, tex->GetTexture());
                }else if(s.type==GR_RENDERTARGET){
                    D3DRenderTarget* t=(D3DRenderTarget*)s.source;
                    shader->SetTexture(d.first,t->GetResource().Get());
                }
                break;
            }
            case GR_DATATYPE_RENDERTARGET:{
                char *texname=reinterpret_cast<char*>(d.second.data);
                std::string tempName(texname,texname+strlen(texname));
                D3DRenderTarget* tex=(D3DRenderTarget*)D3DSourceManager::GetSourceManager()->GetSource(tempName).source;
                std::cout<<"set data:"<<tempName<<std::endl;
                std::cout<<"extra data:"<<d.first<<","<<std::endl;
                shader->SetRenderTarget(d.first,tex->GetRTV().Get());
                break;
            }
            case GR_DATATYPE_MATRIX:{
                XMFLOAT4X4* pm= reinterpret_cast<XMFLOAT4X4*>(d.second.data);
                std::cout<<"extra matrix:"<<*pm<<std::endl;
                shader->SetMatrix4(d.first,*pm);
                break;
            }
            case GR_DATATYPE_STRUCT:{
                shader->SetStruct(d.first,d.second.data,d.second.dataSize);
            }
        }
    }
}

void Scene::InitSprite(SpriteList * sprites) {
    std::vector<SPRITE_VALUE> values;
    std::vector<XMFLOAT3> positions=sprites->GetPositions();
    std::vector<XMFLOAT2> sizes=sprites->GetSizes();
    for(int i=0;i<positions.size();++i){
        values.push_back({positions[i],sizes[i]});
    }

    D3DSourceManager::GetSourceManager()->LoadSpriteList(sprites->GetGameObject()->GetName(),&values[0],values.size(),sprites->GetShaderName(),sprites->GetTextureName(),sprites->GetMaterialName());
}

void Scene::PrintObj(GRCore::GameObject *obj) {
    std::cout<<"+++++++++++start print obj+++++++++"<<std::endl;
    PrintChild(0,obj->GetNode());
}

void Scene::PrintChild(int level,GRCore::Node *node) {
    std::string tab="";
    for(int i=0;i<level;++i){
        tab+="-";
    }
    if(node->obj!=nullptr){
        std::cout << tab+" "+node->obj->GetName() << std::endl;
    }

    if(!node->children.empty()){
        for(auto iter:node->children){
            PrintChild(level+1,iter);
        }
    }
}

XMMATRIX Scene::GetParentModelMatrix(GRCore::Node *node) {
    if(node->parent== nullptr){
        if(node->obj==nullptr){
            return XMMatrixIdentity();
        }else{
            GRCore::Transform* trans=node->obj->GetComponent<Transform>();
            XMFLOAT4X4 temp=trans->GetMatrix();
            XMMATRIX matrix=XMLoadFloat4x4(&temp);
            return matrix;
        }
    }else{
        if(node->obj==nullptr){
            return GetParentModelMatrix(node->parent)*XMMatrixIdentity();
        }
        else {
            GRCore::Transform *trans = node->obj->GetComponent<Transform>();
            XMFLOAT4X4 temp=trans->GetMatrix();
            XMMATRIX matrix=XMLoadFloat4x4(&temp);
            return GetParentModelMatrix(node->parent)*matrix;
        }
    }
}

void Scene::ResetParam() {
    GR_CAMERA_POS=XMFLOAT3(.0f,.0f,.0f);
    GR_MODEL_MATRIX=GR_VIEW_MATRIX=GR_PROJ_MATRIX=DEFAULT_TEX_TRANSFORM;
    GR_MODEL_TEX_TRANSFORM_MATRIX=GR_NORMAL_WORLD_MATRIX=DEFAULT_TEX_TRANSFORM;
}

void Scene::DestroyObject(GRCore::GameObject *obj) {
    if(mRemovedObjects.count(obj)==0) {
        obj->destroy();
        mRemovedObjects.insert(obj);
    }
}

void Scene::DestroyRemovedObject() {
    for(auto obj:mRemovedObjects) {
        RemoveFromRenderQueue(obj);
        std::cout<<"start release node:"<<obj->GetName()<<std::endl;
        ReleaseNode(obj->GetNode());
    }
    mRemovedObjects.clear();
}

void Scene::InitPhysicWorld() {
//    btCollisionConfiguration* collisionConfiguration=new btDefaultCollisionConfiguration();
//    btCollisionDispatcher* dispatcher=new btCollisionDispatcher(collisionConfiguration);
//    btBroadphaseInterface* broadphase=new btDbvtBroadphase();
//    btConstraintSolver* solver=new btSequentialImpulseConstraintSolver();
//
//    mPhysicWorld=new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
//    mPhysicWorld->setGravity(btVector3(0,-10,0));
}

