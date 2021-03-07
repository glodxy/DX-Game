//
// Created by 田淙宇 on 2019/12/3.
//

#ifndef GAME_MESH_H
#define GAME_MESH_H

#include "BaseComponent.h"
#include "../App/D3DMesh.h"

namespace GRCore {
    class Mesh:public BaseComponent{
    public:
        Mesh(const std::string& meshName);
        Mesh(const Mesh&);
        ~Mesh();

        void SetShaderName(const std::string& shader);
        void SetMaterialName(const std::string& mat);
        void SetTexture(TextureData tex,int idx=0);
        void SetModelTexTransform(XMFLOAT4X4 matrix);

        //获取网格名
        std::string GetMeshName()const{
            return mMeshName;
        }
        //获取材质名
        std::string GetMaterialName()const{
            return mMaterialName;
        }
        std::string GetShaderName()const{
            return mShaderName;
        }

        XMFLOAT4X4 GetModelTexTransform(){
            return mTexTransform;
        }
        //获取纹理数据
        std::vector<TextureData> GetTextures()const{
            return mTexs;
        }
        TextureData GetTexture(int idx);

        virtual std::string getName()const{
            return "mesh";
        }

        virtual void awake();
        virtual void update();
    protected:
        std::string mMeshName;
        std::string mMaterialName;
        std::string mShaderName;
        std::vector<TextureData> mTexs;
        XMFLOAT4X4 mTexTransform;
    };
}


#endif //GAME_MESH_H
