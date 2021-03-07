//
// Created by 田淙宇 on 2019/12/5.
//

#include "D3DSourceManager.h"
#include "D3DMesh.h"
#include "D3DTexture.h"
#include "GeometryGenerator.h"
#include "D3DSpriteList.h"
#include "D3DRenderTarget.h"
#include "D3DSprite.h"
#include "D3DAnim.h"

using namespace GRCore;

D3DSourceManager *GRCore::D3DSourceManager::mManager= nullptr;
std::unordered_map<std::string,Source> D3DSourceManager::mSourceMap=std::unordered_map<std::string,Source>();
std::unordered_map<std::string,MeshData> D3DSourceManager::mMeshData=std::unordered_map<std::string,MeshData>();

D3DSourceManager::D3DSourceManager() {
    if(GR_DEVICE== nullptr||GR_CONTEXT== nullptr){
        std::cerr<<"error init source manager:"<<" env not setup"<<std::endl;
    }
    LoadMesh(DEFAULT_CUBE_MESH_NAME,DEFAULT_CUBE_MESHDATA);
    LoadMesh(DEFAULT_PLANE_MESH_NAME,GeometryGenerator::CreateGrid(100.0f,100.0f,100,100));
    LoadMesh(DEFAULT_TERRAIN_MESH_NAME,GeometryGenerator::CreateTerrain());
    LoadMesh(DEFAULT_SKYBOX_MESH_NAME,GeometryGenerator::CreateSphere(5000.0f,30,30));
    LoadMesh(DEFAULT_SPHERE_MESH_NAME,GeometryGenerator::CreateSphere(1.0f,20,20));
    LoadEffect(DEFAULT_SHADER_NAME,DEFAULT_SHADER_FILENAME,DEFAULT_LAYOUT,DEFAULT_LAYOUT_SIZE);
    LoadEffect(DEFAULT_SPRITE_SHADER_NAME,DEFAULT_SPRITE_SHADER_FILENAME,DEFAULT_SPRITE_LAYOUT,DEFAULT_SPRITE_LAYOUT_SIZE);
    LoadEffect("Default_sprite_shader","Shader/Sprite.fx",DEFAULT_SPRITE_LAYOUT,DEFAULT_SPRITE_LAYOUT_SIZE);
    LoadEffect(DEFAULT_SKYBOX_SHADER_NAME,DEFAULT_SKYBOX_SHADER_FILENAME,DEFAULT_SKYBOX_LAYOUT,DEFAULT_SKYBOX_LAYOUT_SIZE);
    LoadEffect(DEFAULT_MODEL_SHADER_NAME,DEFAULT_MODEL_SHADER_FILENAME,DEFAULT_MODEL_LAYOUT,DEFAULT_MODEL_LAYOUT_SIZE);
    LoadMaterial(DEFAULT_MATERIAL_NAME,DEFAULT_MATERIAL);
    LoadTexture(DEFAULT_TEXTURE_WHITE_NAME,TEXTURE_WHITE_FILE);

    LoadRenderTarget(DEFAULT_SHADOW_TEXTURE0_NAME,nullptr,2048,2048);
    LoadRenderTarget(DEFAULT_SHADOW_TEXTURE1_NAME,nullptr,2048,2048);
    LoadRenderTarget(DEFAULT_SHADOW_TEXTURE2_NAME,nullptr,2048,2048);
    LoadRenderTarget(DEFAULT_SHADOW_TEXTURE3_NAME,nullptr,2048,2048);


    LoadSprite();
}


void D3DSourceManager::AddSource(const std::string &name, GRCore::Source s) {
    mSourceMap[name]=s;
}

MeshData D3DSourceManager::GetMeshData(const std::string &meshName) {
    return (mMeshData[meshName]);
}

bool D3DSourceManager::LoadMesh(const std::string &name, MeshData data,BoneData* bone) {
    std::cout<<"create "<<name<<" vcount:"<<data.vertex_count<<std::endl;
    D3DMesh* mesh=new D3DMesh(GR_DEVICE,GR_CONTEXT);
    mesh->Initialize(data.v,data.vertex_count,data.i,data.index_count);
    if(data.n!=nullptr){
        mesh->SetNormalBuffer(data.n,data.normal_count);
    }
    if(data.u!=nullptr){
        mesh->SetUVBuffer(data.u,data.uv_count);
    }
    if(bone!=nullptr){
        mesh->SetBoneBuffer(bone,data.vertex_count);
    }
    Source s{mesh,GR_MESH};
    AddSource(name,s);
    mMeshData[name]=data;
    std::cout<<"create finished"<<std::endl;
    return true;
}

bool D3DSourceManager::LoadEffect(const std::string &name, const std::string &fileName,D3D11_INPUT_ELEMENT_DESC*inlayout,int layoutSize) {
    D3DShader* shader=new D3DShader(GR_DEVICE,GR_WINDOW,GR_CONTEXT);
    shader->InitializeEffect(fileName);
    shader->CreateFXInputLayout(0,inlayout,layoutSize);
    Source s{shader,GR_EFFECT};
    AddSource(name,s);
    return true;
}

bool D3DSourceManager::LoadMaterial(const std::string &name, GRCore::MATERIAL_VALUE material) {
    MATERIAL_VALUE *pm=new MATERIAL_VALUE;
    pm->fresnelR0=material.fresnelR0;
    pm->diffuse=material.diffuse;
    pm->shininess=material.shininess;
    pm->reflect=material.reflect;
    pm->reflection=material.reflection;

    Source s{pm,GR_MATERIAL};
    AddSource(name,s);
    return true;
}

bool D3DSourceManager::LoadTexture(const std::string &name, const std::string &fileName,bool isCube,bool generateMips) {
    std::cout<<"start load texture:"<<fileName<<std::endl;
    D3DTexture* texture=new D3DTexture(GR_DEVICE,GR_CONTEXT);
    texture->Initialize(fileName,isCube,generateMips);

    Source s{texture,GR_TEXTURE};
    AddSource(name,s);
    return true;
}

bool D3DSourceManager::LoadSpriteList(const std::string &name, GRCore::SPRITE_VALUE *values, int count,
                                      const std::string &shader, const std::string &texture,const std::string&material) {
    D3DSpriteList* sprites=new D3DSpriteList(GR_DEVICE, GR_CONTEXT);
    D3DShader* s=(D3DShader*)GetSource(shader).source;
    D3DTexture* t=(D3DTexture*)GetSource(texture).source;
    MATERIAL_VALUE* m=(MATERIAL_VALUE*)GetSource(material).source;
    sprites->Initialize(values,count,t,s,m);

    Source sp{sprites,GR_SPRITES};
    AddSource(name,sp);
    return true;
}

bool D3DSourceManager::LoadRenderTarget(const std::string &name,const McComPtr<ID3D11Texture2D>&tex,int width,int height) {
    D3DRenderTarget* rtv=new D3DRenderTarget(GR_DEVICE,GR_CONTEXT);
    if(!tex) {
        std::cout<<"defalut init rtv"<<std::endl;
        rtv->Initialize(width,height);
    }
    else{
        rtv->Initialize(tex);
    }
    Source s{rtv,GR_RENDERTARGET};
    AddSource(name,s);
    return true;
}

bool D3DSourceManager::LoadSprite() {
    D3DSprite* sprite=new D3DSprite(GR_DEVICE,GR_CONTEXT);
    sprite->Initialize(&DEFAULT_SPRITE_VERTEX);
    Source s{sprite,GR_SPRITE};
    AddSource(DEFAULT_SPRITE_NAME,s);
    return true;
}

bool D3DSourceManager::LoadAnim(const std::string &name, GRCore::D3DAnimMap pos, GRCore::D3DAnimMap rotate,
                                GRCore::D3DAnimMap scale, GRCore::D3DAnimStateMap states,float ticks,float duration) {
    D3DAnim* anim=new D3DAnim();
    anim->Initialize(pos,rotate,scale,states,ticks,duration);
    Source s{anim,GR_ANIMATION};
    AddSource(name,s);
    return true;
}

Source D3DSourceManager::GetSource(const std::string &name) {
    if(mSourceMap.find(name)!=mSourceMap.end()) {
        return mSourceMap[name];
    }
    return {nullptr,GR_NONE};
}

void D3DSourceManager::ResetAll() {
    for(auto iter=mSourceMap.begin();iter!=mSourceMap.end();++iter){
        switch(iter->second.type){
            case GR_TEXTURE:{
                D3DTexture* t=(D3DTexture*)iter->second.source;
                t->Reset();
                delete t;
                break;
            }
            case GR_SPRITE:{
                D3DSprite* s=(D3DSprite*)iter->second.source;
                s->Reset();
                delete s;
                break;
            }
            case GR_MESH:{
                D3DMesh*m=(D3DMesh*)iter->second.source;
                m->Reset();
                delete m;
                break;
            }
            case GR_SPRITES:{
                D3DSpriteList* sl=(D3DSpriteList*)iter->second.source;
                sl->Reset();
                delete sl;
                break;
            }
            case GR_EFFECT:{
                D3DShader* sh=(D3DShader*)iter->second.source;
                sh->Reset();
                delete sh;
                break;
            }
            case GR_RENDERTARGET:{
                D3DRenderTarget* rt=(D3DRenderTarget*)iter->second.source;
                rt->Reset();
                delete rt;
                break;
            }
        }
    }
}