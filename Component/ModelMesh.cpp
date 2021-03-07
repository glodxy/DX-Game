//
// Created by glodxy on 2019/12/23.
//

#include "ModelMesh.h"

using namespace GRCore;

void ModelMesh::AddMesh(GRCore::Mesh m) {
    m.SetShaderName(DEFAULT_MODEL_SHADER_NAME);
    m.SetGameObject(mObject);
    mMeshes.push_back(m);
}

void ModelMesh::SetRootName(std::string name) {
    mRootNode=name;
}

void ModelMesh::SetBoneMatrix(XMFLOAT4X4 matrix, int idx) {
    XMMATRIX s=XMLoadFloat4x4(&mBones[idx].offset);
    s=s*XMLoadFloat4x4(&matrix);
    s=s*XMLoadFloat4x4(&GlobalTransform);
    XMFLOAT4X4 temp;
    XMStoreFloat4x4(&temp,s);
    mBones[idx].finalWorld=temp;
}

void ModelMesh::AddNodeTransform(std::string nodeName, XMFLOAT4X4 trans) {
    mNodeTransform[nodeName]=trans;
}

void ModelMesh::AddMapping(std::string name, int idx) {
    mBoneMapping[name]=idx;
}

void ModelMesh::AddBoneMatrix(GRCore::BoneMatrix m) {
    mBones.push_back(m);
}

void ModelMesh::AddNode(std::string root, std::vector<std::string> children) {
    mBoneNodes[root]=children;
}

void ModelMesh::LoadBonesAndMapping(std::vector<BoneMatrix> bones, std::map<std::string, int> mapping) {
    mBones=bones;
    mBoneMapping=mapping;
}

