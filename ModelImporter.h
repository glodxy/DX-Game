//
// Created by 田淙宇 on 2019/12/16.
//

#ifndef GAME_MODELIMPORTER_H
#define GAME_MODELIMPORTER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>
#include "App/D3DUtil.h"
#include "GameObject/GameObject.h"
#include "GameObject/Scene.h"
#include "Component/ModelMesh.h"

static XMFLOAT4X4 GetMatrix(aiMatrix4x4 matrix){
    XMFLOAT4X4 result;
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            result(i,j)=matrix[j][i];
        }
    }
    return result;
}

static XMFLOAT4 GetFloat4(aiVector3D v){
    XMFLOAT4 result;
    result.x=v.x;
    result.y=v.y;
    result.z=v.z;
    result.w=1.0f;
    return result;
}

static XMFLOAT4 GetFloat4(aiQuaternion quaternion){
    XMFLOAT4 result;
    result.x=quaternion.x;
    result.y=quaternion.y;
    result.z=quaternion.z;
    result.w=quaternion.w;
    return result;
}

class ModelImporter {
public:

    GRCore::GameObject* LoadModel(std::string path,std::string name,GRCore::Scene*scene);
    void LoadAnim(std::string path,std::string name,int idx);
private:
    void ProcessNode(GRCore::GameObject*obj,aiNode*node,const aiScene* scene,GRCore::ModelMesh* mm);
    void ProcessMesh(aiMesh*mesh,const aiScene*scene,GRCore::ModelMesh*mm);
    void ProcessAnimNode(aiNode* node,aiAnimation*anim,GRCore::D3DAnimMap& pos,GRCore::D3DAnimMap&rotation,GRCore::D3DAnimMap& scale,GRCore::D3DAnimStateMap& states);
    std::vector<GRCore::TextureData> ProcessTexture(aiMaterial*mat,aiTextureType type,std::string typeName);

    std::string mDirPath;

};


#endif //GAME_MODELIMPORTER_H
