//
// Created by 田淙宇 on 2019/12/2.
//

#ifndef GAME_D3DMESH_H
#define GAME_D3DMESH_H

#include "D3DUtil.h"
#include "../Math/DXMath.h"
#include "D3DShader.h"
#include "D3DTexture.h"

namespace GRCore {
    class D3DMesh {
    public:
        D3DMesh(ID3D11Device *device, ID3D11DeviceContext *context);

        D3DMesh(const D3DMesh &m)=delete;

        void Reset();


        ~D3DMesh();

    public:
        bool Initialize(Vertex*v,int vCount,Index*i,int iCount);

        bool SetNormalBuffer(Normal*n,int ncount);
        bool SetUVBuffer(UV*u,int uvcount);
        bool SetBoneBuffer(BoneData* bone,int count);
        void Render(D3DShader* s,std::vector<ID3D11ShaderResourceView*> texture,std::vector<XMFLOAT4X4> texTrans,MATERIAL_VALUE*m,ID3D11ShaderResourceView*skybox=nullptr);

        void Render(D3DShader*s,std::vector<ID3D11ShaderResourceView*> texture,std::vector<XMFLOAT4X4> texTrans,MATERIAL_VALUE*m,XMFLOAT4X4* bones,bool useBones=true,ID3D11ShaderResourceView*skybox=nullptr);
        int GetVertexCount()const{
            return mVertexCount;
        }

        int GetIndexCount()const{
            return mIndexCount;
        }

        ID3D11Buffer* GetVertexBuffer(){
            return mVertexBuffer;
        }

        ID3D11Buffer* GetIndexBuffer(){
            return mIndexBuffer;
        }

    protected:
        ID3D11Device *mDevice;
        ID3D11DeviceContext* mContext;
        ID3D11Buffer* mVertexBuffer;
        ID3D11Buffer* mNormalBuffer=nullptr;
        ID3D11Buffer* mUvBuffer=nullptr;
        ID3D11Buffer* mIndexBuffer;
        ID3D11Buffer* mBoneBuffer=nullptr;
        int mVertexCount;
        int mIndexCount;
        int mNormalCount;
        int mUvCount;
        int mBoneCount;
    };
}





#endif //GAME_D3DMESH_H
