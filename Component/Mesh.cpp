//
// Created by 田淙宇 on 2019/12/3.
//

#include "Mesh.h"
#include "../GameObject/GameObject.h"
#include "Transform.h"
#include "../App/D3DSourceManager.h"

using namespace GRCore;

Mesh::Mesh(const std::string &meshName) {
    mMeshName=meshName;
    mTexs.push_back({DEFAULT_TEXTURE_WHITE_NAME,DEFAULT_TEX_TRANSFORM});
    mMaterialName=DEFAULT_MATERIAL_NAME;
    mTexTransform=DEFAULT_TEX_TRANSFORM;
    mShaderName=DEFAULT_SHADER_NAME;
}

Mesh::Mesh(const GRCore::Mesh &m) {
    mMeshName=m.mMeshName;
    mMaterialName=m.mMaterialName;
    mShaderName=m.mShaderName;
    mTexTransform=m.mTexTransform;
    mTexs=m.mTexs;
    mObject=m.mObject;
}

void Mesh::SetShaderName(const std::string &shader) {
    mShaderName=shader;
}

void Mesh::SetTexture(GRCore::TextureData tex, int idx) {
    if(idx>=mTexs.size()){
        std::cerr<<"set texture size error"<<std::endl;
        mTexs.resize(idx+1);
    }
    mTexs[idx]=tex;
}

void Mesh::SetModelTexTransform(XMFLOAT4X4 matrix) {
    mTexTransform=matrix;
}

void Mesh::SetMaterialName(const std::string &mat) {
    mMaterialName=mat;
}

TextureData Mesh::GetTexture(int idx) {
    return mTexs[idx];
}

Mesh::~Mesh(){
}

void Mesh::awake() {

}

void Mesh::update() {

}

