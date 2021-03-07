//
// Created by 田淙宇 on 2019/9/9.
//

#ifndef GRENDERER_D3DUTIL_H
#define GRENDERER_D3DUTIL_H

#include <Windows.h>
#include <wrl.h>
#include <dxgi1_4.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cassert>

#include <d3dx11effect.h>
#include <map>

#include <DirectXTex.h>


#include "../Math/DXMath.h"
#include "../BaseApp.h"
#include "Mouse.h"
#include "Keyboard.h"

namespace GRCore {

    typedef struct{
        XMFLOAT3 diffuse;
        float spot;
        XMFLOAT3 position;
        float falloffStart;
        XMFLOAT3 direction;
        float falloffEnd;
    }LIGHT;

    typedef struct{
        XMFLOAT4 diffuse;
        XMFLOAT4 reflection;
        XMFLOAT3 fresnelR0;
        float shininess;
        int reflect;
    }MATERIAL_VALUE;

    typedef struct {
        XMFLOAT3 pos;
        XMFLOAT2 size;
    }SPRITE_VALUE;

    extern ID3D11Device* GR_DEVICE;
    extern ID3D11DeviceContext* GR_CONTEXT;
    extern HWND GR_WINDOW;
    extern int GR_WIDTH;
    extern int GR_HEIGHT;
    extern int GR_MSAA_QUALITY;

    extern std::string GR_CURRENT_MODE;

    extern D3D11_VIEWPORT GR_CURRENT_VIEWPORT;

    extern int GR_CULLING_MASK;
    extern XMFLOAT3 GR_CAMERA_POS;
    extern XMFLOAT4X4 GR_VIEW_MATRIX;
    extern XMFLOAT4X4 GR_PROJ_MATRIX;
    extern XMFLOAT4X4 GR_MODEL_MATRIX;
    extern XMFLOAT4X4 GR_NORMAL_WORLD_MATRIX;//将法线转世界坐标系的矩阵
    extern XMFLOAT4X4 GR_MODEL_TEX_TRANSFORM_MATRIX;//单个物体的纹理偏移
    extern XMFLOAT4X4 GR_LIGHT_VIEWPROJ_MATRIX[4];//光线空间的变换矩阵

    extern XMFLOAT4X4* GR_BONES_MATRIX;

    extern XMFLOAT4 GR_AMBIENT_LIGHT;
    extern LIGHT GR_LIGHTS[30];
    extern int GR_DIRLIGHT_SIZE;//方向光数量
    extern int GR_POINTLIGHT_SIZE;//点光源数目
    extern int GR_SPOTLIGHT_SIZE;//聚光数量

    extern std::string GR_SKYBOX_TEXTURE;

    extern Mouse *GR_MOUSE;
    extern Keyboard* GR_KEYBOARD;


    extern GameTimer* GR_TIMER;


    inline std::wstring AnsiToWString(const std::string &str) {
        WCHAR buffer[512];

        MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
        return std::wstring(buffer);
    }

    inline std::string WStringToAnsi(const std::wstring &str) {
        CHAR buffer[512];
        WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, buffer, 512, NULL, NULL);
        return std::string(buffer);
    }

    inline XMFLOAT3 Mul(XMFLOAT3 v,float s){
        return XMFLOAT3(v.x*s,v.y*s,v.z*s);
    }

    inline XMFLOAT3 Add(XMFLOAT3 a,XMFLOAT3 b){
        return XMFLOAT3(a.x+b.x,a.y+b.y,a.z+b.z);
    }

    inline XMFLOAT3 Sub(XMFLOAT3 a,XMFLOAT3 b){
        return XMFLOAT3(a.x-b.x,a.y-b.y,a.z-b.z);
    }



    typedef XMFLOAT3 Vertex;
    typedef XMFLOAT3 Normal;
    typedef XMFLOAT2 UV;
    typedef WORD Index;

    typedef struct{
        Vertex* v;
        Index* i;
        Normal *n=nullptr;
        UV* u=nullptr;
        int vertex_count;
        int index_count;
        int normal_count=0;
        int uv_count=0;
    }MeshData;

    typedef struct{
        std::string tex;
        XMFLOAT4X4 texTrans;
    }TextureData;

    typedef struct{
        XMFLOAT4X4 offset;
        XMFLOAT4X4 finalWorld;
    }BoneMatrix;

    typedef struct _BoneData{
        //只处理16块骨骼
        int ids[16];
        float weights[16];
        _BoneData(){
            memset(ids,0, sizeof(ids));
            memset(weights,0, sizeof(weights));
        }
        void AddBoneData(int id,float weight){
            for (int i = 0; i < 16; i++)
            {
                if (weights[i] == 0.0)
                {
                    ids[i] = id;
                    weights[i] = weight;
                    return;
                }
            }
        }
    }BoneData;

    typedef struct{
        float time;
        XMFLOAT4 transform;
    }AnimData;

    typedef struct{
        int preState;
        int postState;
    }NodeAnimState;

    typedef std::map<std::string,std::vector<AnimData>> D3DAnimMap;
    typedef std::map<std::string,NodeAnimState> D3DAnimStateMap;


    typedef struct{
        float Amplitude;
        float Frequency;
        float Length;
        float Speed;
        XMFLOAT2 Direction;
    }Wave;

    enum ExtraDataType{
        GR_DATATYPE_NONE,
        GR_DATATYPE_FLOAT3,
        GR_DATATYPE_FLOAT2,
        GR_DATATYPE_FLOAT,
        GR_DATATYPE_INT,
        GR_DATATYPE_BOOL,
        GR_DATATYPE_RENDERTARGET,
        GR_DATATYPE_TEXTURE,
        GR_DATATYPE_MATRIX,
        GR_DATATYPE_STRUCT,
    };

    class ExtraData{
    public:
        void ClearData();
        ExtraData();
        ExtraData(void* p,int size);
        ExtraData(const std::string&data,bool rtv=false);
        ExtraData(XMFLOAT4X4 data);
        ExtraData(XMFLOAT3 data);
        ExtraData(XMFLOAT2 data);
        ExtraData(int data);
        ExtraData(ExtraDataType type);
        ExtraData(const ExtraData&d);
        ExtraData&operator=(const ExtraData&r);
        ~ExtraData();
        ExtraDataType dataType;
        int dataSize;
        void* data;
    private:
    };

    enum RenderType{RT_BackGround,RT_Geometry,RT_AlphaTest,RT_Transparent,RT_UI};

    static const std::string GR_DEFAULT_MODE="";
    static const std::string GR_ALPHA_TEST_MODE="_AlphaTest";
    static const std::string GR_TRANSPARENT_MODE="_Transparent";
    static const std::string GR_UI_MODE="_UI";
    static const std::string GR_SHADOW_MODE="_Shadow";

    static const int DIR_LIGHT_TYPE=0;
    static const int POINT_LIGHT_TYPE=1;
    static const int SPOT_LIGHT_TYPE=2;

    static const int GR_DEFAULT_LAYER=1;

    static XMFLOAT4 COLOR_BLACK=XMFLOAT4(0.0f,0.0f,0.0f,1.0f);
    static XMFLOAT4 COLOR_WHITE=XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
    static XMFLOAT4 COLOR_RED=XMFLOAT4(1.0f,0.0f,0.0f,1.0f);
    static XMFLOAT4 COLOR_GREEN=XMFLOAT4(0.0f,1.0f,0.0f,1.0f);
    static XMFLOAT4 COLOR_BLUE=XMFLOAT4(0.0f,0.0f,1.0f,1.0f);

    static XMFLOAT4X4 DEFAULT_TEX_TRANSFORM=XMFLOAT4X4(1.0f,.0f,.0f,.0f,.0f,1.0f,.0f,.0f,.0f,.0f,1.0f,.0f,.0f,.0f,.0f,1.0f);
    static XMFLOAT4X4 DEFAULT_ZERO_MATRIX=XMFLOAT4X4(.0f,.0f,.0f,.0f,.0f,.0f,.0f,.0f,.0f,.0f,.0f,.0f,.0f,.0f,.0f,.0f);

    const static std::string TEXTURE_WHITE_FILE="Texture/white.jpg";
    const static std::string DEFAULT_TEXTURE_WHITE_NAME="TextureWhite";

    static const std::string DEFAULT_SHADER_FILENAME="Shader/simple.fx";
    static const std::string DEFAULT_SPRITE_SHADER_FILENAME="Shader/SpriteBillBoard.fx";
    static const std::string DEFAULT_SKYBOX_SHADER_FILENAME="Shader/SkyBox.fx";
    static const std::string DEFAULT_MODEL_SHADER_FILENAME="Shader/Model.fx";

    static const std::string DEFAULT_SHADER_NAME="DefaultShader";
    static const std::string DEFAULT_SPRITE_SHADER_NAME="DefaultSpriteShader";
    static const std::string DEFAULT_SKYBOX_SHADER_NAME="DefaultSkyboxShader";
    static const std::string DEFAULT_MODEL_SHADER_NAME="DefaultModelShader";
    static const std::string DEFAULT_SKYBOX_MESH_NAME="SkyboxMesh";
    static const std::string DEFAULT_CUBE_MESH_NAME="CubeMesh";
    static const std::string DEFAULT_TERRAIN_MESH_NAME="TerrainMesh";
    static const std::string DEFAULT_PLANE_MESH_NAME="PlaneMesh";
    static const std::string DEFAULT_SPHERE_MESH_NAME="SphereMesh";
    static const std::string DEFAULT_MATERIAL_NAME="DefaultMaterial";
    static const std::string DEFAULT_RENDERTARGET_NAME="DefaultRenderTarget";
    static const std::string DEFAULT_SPRITE_NAME="DefaultSprite";
    static const std::string DEFAULT_SHADOW_TEXTURE_NAME="DefaultShadowMap";
    static const std::string DEFAULT_SHADOW_TEXTURE0_NAME="DefaultShadowMap0";
    static const std::string DEFAULT_SHADOW_TEXTURE1_NAME="DefaultShadowMap1";
    static const std::string DEFAULT_SHADOW_TEXTURE2_NAME="DefaultShadowMap2";
    static const std::string DEFAULT_SHADOW_TEXTURE3_NAME="DefaultShadowMap3";

    static const int MAX_BONES=100;

    class DxException {
    public:
        DxException() = default;

        DxException(HRESULT hr, const std::wstring &functionName, const std::wstring &filename, int lineNumber);

        std::wstring ToString() const;

        HRESULT ErrorCode = S_OK;
        std::wstring FunctionName;
        std::wstring Filename;
        int LineNumber = -1;
    };

    template<class T>
    using McComPtr= Microsoft::WRL::ComPtr<T>;
}

#ifndef ThrowIfFailed
#define ThrowIfFailed(x)                                              \
{                                                                     \
    HRESULT hr__ = (x);                                               \
    std::wstring wfn = AnsiToWString(__FILE__);                       \
    if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
}
#endif

#ifndef ReleaseCom
#define ReleaseCom(x) { if(x){ x->Release(); x = 0; } }
#endif

#ifndef _DEBUG_OUT
#define DebugLog(x) { #x }
#else
#define DebugLog(x){std::cout<<x<<std::endl;}
#endif


#endif //GRENDERER_D3DUTIL_H
