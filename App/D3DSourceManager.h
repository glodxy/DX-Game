//
// Created by 田淙宇 on 2019/12/5.
//

#ifndef GAME_D3DSOURCEMANAGER_H
#define GAME_D3DSOURCEMANAGER_H

#include "D3DUtil.h"
#include <unordered_map>

namespace GRCore {
    enum SOURCE_TYPE{GR_NONE,GR_MESH,GR_EFFECT,GR_MATERIAL,GR_TEXTURE,GR_SPRITE,GR_SPRITES,GR_RENDERTARGET,GR_ANIMATION};

    typedef struct{
        void* source;
        SOURCE_TYPE type;
    }Source;
    class D3DSourceManager {
    public:
        static D3DSourceManager* GetSourceManager(){
            if(mManager==nullptr){
                mManager=new D3DSourceManager();
            }
            return mManager;
        }

        bool LoadMesh(const std::string& name,MeshData data,BoneData*bone=nullptr);
        bool LoadEffect(const std::string& name,const std::string& fileName,D3D11_INPUT_ELEMENT_DESC* inlayout,int layoutSize);
        bool LoadMaterial(const std::string& name,MATERIAL_VALUE material);
        bool LoadTexture(const std::string& name,const std::string& fileName,bool isCube=false,bool generateMips=false);
        bool LoadRenderTarget(const std::string& name,const McComPtr<ID3D11Texture2D>& tex,int width=GR_WIDTH,int height=GR_HEIGHT);
        bool LoadSpriteList(const std::string&name,SPRITE_VALUE* values,int count,const std::string&shader,const std::string&texture,const std::string&material);
        bool LoadAnim(const std::string&name,D3DAnimMap pos,D3DAnimMap rotate,D3DAnimMap scale,D3DAnimStateMap states,float ticks,float duration);
        //获取资源，获取后自行判断类型
        Source GetSource(const std::string& name);

        MeshData GetMeshData(const std::string&meshName);

        static void ResetAll();

    private:
        D3DSourceManager();
        D3DSourceManager(const D3DSourceManager&)=delete;
        bool LoadSprite();
        void AddSource(const std::string& name,Source s);
        static D3DSourceManager* mManager;
        static std::unordered_map<std::string,Source> mSourceMap;
        static std::unordered_map<std::string,MeshData> mMeshData;
    };

    static D3D11_INPUT_ELEMENT_DESC DEFAULT_LAYOUT[]={
            { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }, // 96位即12个字节
            { "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,1,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
            {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,2,0,D3D11_INPUT_PER_VERTEX_DATA,0}
    };
    static const int DEFAULT_LAYOUT_SIZE= sizeof(DEFAULT_LAYOUT);
    static D3D11_INPUT_ELEMENT_DESC DEFAULT_SPRITE_LAYOUT[]={
            {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
    };
    static const int DEFAULT_SPRITE_LAYOUT_SIZE= sizeof(DEFAULT_SPRITE_LAYOUT);

    static D3D11_INPUT_ELEMENT_DESC DEFAULT_MODEL_LAYOUT[]={
            { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }, // 96位即12个字节
            { "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,1,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
            {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,2,0,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"TEXCOORD",1,DXGI_FORMAT_R32G32B32A32_UINT,3,0,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"TEXCOORD",2,DXGI_FORMAT_R32G32B32A32_UINT,3,16,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"TEXCOORD",3,DXGI_FORMAT_R32G32B32A32_UINT,3,32,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"TEXCOORD",4,DXGI_FORMAT_R32G32B32A32_UINT,3,48,D3D11_INPUT_PER_VERTEX_DATA,0},
            //权重
            {"TEXCOORD",5,DXGI_FORMAT_R32G32B32A32_FLOAT,3,64,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"TEXCOORD",6,DXGI_FORMAT_R32G32B32A32_FLOAT,3,80,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"TEXCOORD",7,DXGI_FORMAT_R32G32B32A32_FLOAT,3,96,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"TEXCOORD",8,DXGI_FORMAT_R32G32B32A32_FLOAT,3,112,D3D11_INPUT_PER_VERTEX_DATA,0},
    };
    static const int DEFAULT_MODEL_LAYOUT_SIZE= sizeof(DEFAULT_MODEL_LAYOUT);

    static D3D11_INPUT_ELEMENT_DESC DEFAULT_SKYBOX_LAYOUT[]={
            {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0}
    };
    static const int DEFAULT_SKYBOX_LAYOUT_SIZE= sizeof(DEFAULT_SKYBOX_LAYOUT);

    static Vertex DEFAULT_CUBE_VERTEX[]={
            {XMFLOAT3(-1.0f,1.0f,-1.0f)},
            {XMFLOAT3(1.0f,1.0f,-1.0f)},
            {XMFLOAT3(1.0f,1.0f,1.0f)},
            {XMFLOAT3(-1.0f,1.0f,1.0f)},
            {XMFLOAT3(-1.0f,-1.0f,-1.0f)},
            {XMFLOAT3(1.0f,-1.0f,-1.0f)},
            {XMFLOAT3(1.0f,-1.0f,1.0f)},
            {XMFLOAT3(-1.0f,-1.0f,1.0f)},
            XMFLOAT3(-1.0f,-1.0f,1.0f),
            XMFLOAT3(-1.f,-1.f,-1.f),
            XMFLOAT3(-1.f,1.f,-1.f),
            XMFLOAT3(-1.f,1.f,1.f),

            XMFLOAT3(1.f,-1.f,1.f),
            XMFLOAT3(1.f,-1.f,-1.f),
            XMFLOAT3(1.f,1.f,-1.f),
            XMFLOAT3(1.f,1.f,1.f),

            XMFLOAT3(-1.f,-1.f,-1.f),
            XMFLOAT3(1.f,-1.f,-1.f),
            XMFLOAT3(1.f,1.f,-1.f),
            XMFLOAT3(-1.f,1.f,-1.f),

            XMFLOAT3(-1.f,-1.f,1.f),
            XMFLOAT3(1.f,-1.f,1.f),
            XMFLOAT3(1.f,1.f,1.f),
            XMFLOAT3(-1.f,1.f,1.f)
    };
    static const int DEFAULT_CUBE_VERTEX_SIZE=sizeof(DEFAULT_CUBE_VERTEX)/ sizeof(Vertex);

    static Normal DEFAULT_CUBE_NORMAL[]={
            XMFLOAT3(.0f,1.f,.0f),
            XMFLOAT3(0.f,1.f,0.f),
            XMFLOAT3(0.f,1.f,0.f),
            XMFLOAT3(0.f,1.f,0.f),

            XMFLOAT3(0.f,-1.f,0.f),
            XMFLOAT3(0.f,-1.f,0.f),
            XMFLOAT3(0.f,-1.f,0.f),
            XMFLOAT3(0.f,-1.f,0.f),

            XMFLOAT3(-1.f,0.f,0.f),
            XMFLOAT3(-1.f,0.f,0.f),
            XMFLOAT3(-1.f,0.f,0.f),
            XMFLOAT3(-1.f,0.f,0.f),

            XMFLOAT3(1.f,0.f,0.f),
            XMFLOAT3(1.f,0.f,0.f),
            XMFLOAT3(1.f,0.f,0.f),
            XMFLOAT3(1.f,0.f,0.f),

            XMFLOAT3(0.f,0.f,-1.f),
            XMFLOAT3(0.f,0.f,-1.f),
            XMFLOAT3(0.f,0.f,-1.f),
            XMFLOAT3(0.f,0.f,-1.f),

            XMFLOAT3(0.f,0.f,1.f),
            XMFLOAT3(0.f,0.f,1.f),
            XMFLOAT3(0.f,0.f,1.f),
            XMFLOAT3(0.f,0.f,1.f),
    };
    static const int DEFAULT_CUBE_NORMAL_SIZE= sizeof(DEFAULT_CUBE_NORMAL)/ sizeof(Normal);

    static UV DEFAULT_CUBE_UV[]={
            XMFLOAT2(.0f,.0f),
            XMFLOAT2(1.0f,.0f),
            XMFLOAT2(1.f,1.f),
            XMFLOAT2(.0f,1.f),

            XMFLOAT2(.0f,.0f),
            XMFLOAT2(1.0f,.0f),
            XMFLOAT2(1.f,1.f),
            XMFLOAT2(.0f,1.f),

            XMFLOAT2(.0f,.0f),
            XMFLOAT2(1.0f,.0f),
            XMFLOAT2(1.f,1.f),
            XMFLOAT2(.0f,1.f),

            XMFLOAT2(.0f,.0f),
            XMFLOAT2(1.0f,.0f),
            XMFLOAT2(1.f,1.f),
            XMFLOAT2(.0f,1.f),

            XMFLOAT2(.0f,.0f),
            XMFLOAT2(1.0f,.0f),
            XMFLOAT2(1.f,1.f),
            XMFLOAT2(.0f,1.f),

            XMFLOAT2(.0f,.0f),
            XMFLOAT2(1.0f,.0f),
            XMFLOAT2(1.f,1.f),
            XMFLOAT2(.0f,1.f),
    };
    static const int DEFAULT_CUBE_UV_SIZE= sizeof(DEFAULT_CUBE_UV)/ sizeof(UV);


    static Index DEFAULT_CUBE_INDEX[]={
            3,1,0, 2,1,3, 6,4,5, 7,4,6, 11,9,8, 10,9,11,
            14,12,13, 15,12,14, 19,17,16, 18,17,19, 22,20,21, 23,20,22
    };
    static const int DEFAULT_CUBE_INDEX_SIZE=sizeof(DEFAULT_CUBE_INDEX)/sizeof(Index);

    static const MATERIAL_VALUE DEFAULT_MATERIAL={
            XMFLOAT4(1.0f,1.0f,1.0f,1.0f),
            XMFLOAT4(1.0f,1.0f,1.0f,1.0f),
            XMFLOAT3(1.5f,1.5f,1.5f),
            1.0f,
            0
    };

    static const MeshData DEFAULT_CUBE_MESHDATA={
            DEFAULT_CUBE_VERTEX,
            DEFAULT_CUBE_INDEX,
            DEFAULT_CUBE_NORMAL,
            DEFAULT_CUBE_UV,
            DEFAULT_CUBE_VERTEX_SIZE,
            DEFAULT_CUBE_INDEX_SIZE,
            DEFAULT_CUBE_NORMAL_SIZE,
            DEFAULT_CUBE_UV_SIZE
    };

    static const SPRITE_VALUE DEFAULT_SPRITE_VERTEX={
            XMFLOAT3(.0f,.0f,.0f),
            XMFLOAT2(1.0f,1.0f)
    };
}


#endif //GAME_D3DSOURCEMANAGER_H
