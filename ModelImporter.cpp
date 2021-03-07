//
// Created by 田淙宇 on 2019/12/16.
//

#include "ModelImporter.h"
#include "GameObject/GameObject.h"
#include "Component/Mesh.h"
#include "App/D3DSourceManager.h"
#include "Component/ModelMesh.h"

using namespace GRCore;

void ModelImporter::LoadAnim(std::string path, std::string name,int idx) {
    mDirPath=path.substr(0,path.find_last_of('/'));
    Assimp::Importer importer;
    std::cout<<"start read from file"<<std::endl;
    const aiScene* scene=importer.ReadFile(path,aiProcess_Triangulate);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cout << "错误::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    std::cout<<"animation nums:"<<scene->mNumAnimations<<std::endl;
    for(int i=0;i<scene->mNumAnimations;++i){
        std::cout<<scene->mAnimations[i]->mName.data<<std::endl;
        std::cout<<"time:"<<scene->mAnimations[i]->mDuration<<std::endl;
    }
    if(scene->mNumAnimations<=idx){
        std::cerr<<"this file do not have animation in "<<idx<<std::endl;
        std::cerr<<"all animations:"<<scene->mNumAnimations<<std::endl;
        return;
    }

    aiAnimation* animation=scene->mAnimations[idx];
    float ticks=animation->mTicksPerSecond;
    float duration=animation->mDuration;
    std::cout<<"start load animation:"<<animation->mName.data<<std::endl;
    D3DAnimMap pos,rotation,scale;
    D3DAnimStateMap states;
    ProcessAnimNode(scene->mRootNode,animation,pos,rotation,scale,states);
    D3DSourceManager::GetSourceManager()->LoadAnim(name,pos,rotation,scale,states,ticks,duration);
}

void ModelImporter::ProcessAnimNode(aiNode *node,aiAnimation* anim,GRCore::D3DAnimMap& pos,GRCore::D3DAnimMap&rotation,GRCore::D3DAnimMap& scale,GRCore::D3DAnimStateMap& states) {
    std::string nodeName(node->mName.data);
    //获取节点动画
    aiNodeAnim* nodeAnim=nullptr;
    for(int i=0;i<anim->mNumChannels;++i){
        if(nodeName==anim->mChannels[i]->mNodeName.data){
            nodeAnim=anim->mChannels[i];
            break;
        }
    }

    if(nodeAnim!=nullptr){
        std::vector<AnimData> positionAnim;
        std::vector<AnimData> rotationAnim;
        std::vector<AnimData> scaleAnim;
        NodeAnimState state;
        state.preState=nodeAnim->mPreState;
        state.postState=nodeAnim->mPostState;

        AnimData data;
        //读取坐标变换数据
        for(int i=0;i<nodeAnim->mNumPositionKeys;++i){
            data.time=nodeAnim->mPositionKeys[i].mTime;
            data.transform=GetFloat4(nodeAnim->mPositionKeys[i].mValue);
            positionAnim.push_back(data);
        }

        //读取旋转
        for(int i=0;i<nodeAnim->mNumRotationKeys;++i){
            data.time=nodeAnim->mRotationKeys[i].mTime;
            data.transform=GetFloat4(nodeAnim->mRotationKeys[i].mValue);
            rotationAnim.push_back(data);
        }

        //读取缩放
        for(int i=0;i<nodeAnim->mNumScalingKeys;++i){
            data.time=nodeAnim->mScalingKeys[i].mTime;
            data.transform=GetFloat4(nodeAnim->mScalingKeys[i].mValue);
            scaleAnim.push_back(data);
        }

        pos[nodeName]=positionAnim;
        rotation[nodeName]=rotationAnim;
        scale[nodeName]=scaleAnim;
        states[nodeName]=state;
    }

    if(node->mNumChildren>0){
        for(unsigned int i=0;i<node->mNumChildren;++i){
            ProcessAnimNode(node->mChildren[i],anim,pos,rotation,scale,states);
        }
    }
}

GameObject* ModelImporter::LoadModel(std::string path, std::string name,GRCore::Scene *targetScene) {
    mDirPath=path.substr(0,path.find_last_of('/'));
    Assimp::Importer importer;
    std::cout<<"start read from file"<<std::endl;
    const aiScene* scene=importer.ReadFile(path,aiProcess_Triangulate);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cout << "错误::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }
    GameObject *obj=new GameObject();
    obj->SetName(name);
    targetScene->AddChild(nullptr,obj);
    std::cout<<"obj init finished"<<std::endl;

    ModelMesh* mm=new ModelMesh();
    obj->AddComponent(mm);
    mm->SetRootName(scene->mRootNode->mName.data);
    mm->GlobalTransform=GetMatrix(scene->mRootNode->mTransformation.Inverse());
    ProcessNode(obj,scene->mRootNode,scene,mm);

    mm->PrintNode();
    return obj;
}

void ModelImporter::ProcessNode(GameObject*obj,aiNode *node, const aiScene *scene,ModelMesh*mm) {
    for(unsigned int i=0;i<node->mNumMeshes;++i){
        aiMesh*mesh=scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh,scene,mm);
    }
    mm->AddNodeTransform(node->mName.data,GetMatrix(node->mTransformation));
    if(node->mNumChildren>0){
        std::vector<std::string> childNames;
        for(int i=0;i<node->mNumChildren;++i){
            childNames.push_back(node->mChildren[i]->mName.data);
        }
        mm->AddNode(node->mName.data,childNames);
        for(unsigned int i=0;i<node->mNumChildren;++i){
            ProcessNode(nullptr,node->mChildren[i],scene,mm);
        }
    }
}

void ModelImporter::ProcessMesh(aiMesh *mesh, const aiScene *scene,ModelMesh*mm) {
    std::string meshName=std::string(mesh->mName.C_Str())+"_mesh";
    std::cout<<"meshName:"<<meshName<<std::endl;
    Mesh pm(meshName);
    std::vector<Vertex> vertices;
    std::vector<Normal> normals;
    std::vector<UV> uvs;
    std::vector<Index> indices;
    std::vector<BoneData> bones;
    bones.resize(mesh->mNumVertices,BoneData());
    std::cout<<"vertices num:"<<mesh->mNumVertices<<std::endl;

    for(unsigned int i=0;i<mesh->mNumVertices;++i){
        Vertex vertex;
        vertex.x=mesh->mVertices[i].x;
        vertex.y=mesh->mVertices[i].y;
        vertex.z=mesh->mVertices[i].z;

        Normal normal;
        normal.x=mesh->mNormals[i].x;
        normal.y=mesh->mNormals[i].y;
        normal.z=mesh->mNormals[i].z;

        UV uv;
        if(mesh->mTextureCoords[0]){
            uv.x=mesh->mTextureCoords[0][i].x;
            uv.y=mesh->mTextureCoords[0][i].y;
        }
        else{
            uv=XMFLOAT2(.0f,.0f);
        }

        vertices.push_back(vertex);
        normals.push_back(normal);
        uvs.push_back(uv);
    }

    for(unsigned int i=0;i<mesh->mNumFaces;++i){
        aiFace face=mesh->mFaces[i];
        for(unsigned int j=0;j<face.mNumIndices;++j){
            indices.push_back(face.mIndices[j]);
        }
    }

    std::cout<<"bones num:"<<mesh->mNumBones<<std::endl;
    for(unsigned int i=0;i<mesh->mNumBones;++i){
        int index=0;
        std::string boneName(mesh->mBones[i]->mName.data);

        if(mm->GetBoneIdx(boneName)==-1){
            index=mm->GetBoneSize();
            BoneMatrix m;
            m.offset=GetMatrix(mesh->mBones[i]->mOffsetMatrix);
            m.finalWorld=DEFAULT_ZERO_MATRIX;
            mm->AddMapping(boneName,index);
            mm->AddBoneMatrix(m);
        }
        else{
            index=mm->GetBoneIdx(boneName);
        }

        for(unsigned int j=0;j<mesh->mBones[i]->mNumWeights;++j){
            int vertexID=mesh->mBones[i]->mWeights[j].mVertexId;
            float weight=mesh->mBones[i]->mWeights[j].mWeight;
            bones[vertexID].AddBoneData(index,weight);
        }
    }

    int vsize=vertices.size();
    int isize=indices.size();
    int nsize=normals.size();
    int usize=uvs.size();
    MeshData meshdata={
            &vertices[0],
            &indices[0],
            &normals[0],
            &uvs[0],
            vsize,
            isize,
            nsize,
            usize
    };
    D3DSourceManager::GetSourceManager()->LoadMesh(meshName,meshdata,&bones[0]);

    aiMaterial* material=scene->mMaterials[mesh->mMaterialIndex];
    std::cout<<"start create texture:"<<std::endl;
    std::vector<TextureData> diffuseMap=ProcessTexture(material,aiTextureType_DIFFUSE,"_diffuseMap");
    if(diffuseMap.empty()){
        std::cout<<"start create ambient texture"<<std::endl;
        diffuseMap=ProcessTexture(material,aiTextureType_AMBIENT,"_ambientMap");
    }
    if(diffuseMap.empty()){
        std::cout<<"start create specular texture"<<std::endl;
        diffuseMap=ProcessTexture(material,aiTextureType_SPECULAR,"_ambientMap");
    }
    if(diffuseMap.empty()){
        std::cout<<"start create opacity texture"<<std::endl;
        diffuseMap=ProcessTexture(material,aiTextureType_OPACITY,"_ambientMap");
    }
    if(diffuseMap.empty()){
        std::cout<<"start create none texture"<<std::endl;
        diffuseMap=ProcessTexture(material,aiTextureType_NONE,"_ambientMap");
    }
    if(diffuseMap.empty()){
        std::cout<<"start create opacity texture"<<std::endl;
        diffuseMap=ProcessTexture(material,aiTextureType_REFLECTION,"_ambientMap");
    }
    if(diffuseMap.empty()){
        std::cout<<"start create unknown texture"<<std::endl;
        diffuseMap=ProcessTexture(material,aiTextureType_UNKNOWN,"_unknownMap");
    }
    if(diffuseMap.empty()){
        aiColor3D color;
        material->Get(AI_MATKEY_COLOR_DIFFUSE,color);
        MATERIAL_VALUE value=DEFAULT_MATERIAL;
        value.diffuse=XMFLOAT4(color.r,color.g,color.b,1.0f);
        D3DSourceManager::GetSourceManager()->LoadMaterial(meshName+"_material",value);
        pm.SetMaterialName(meshName+"_material");
    }
    if(!diffuseMap.empty())
        pm.SetTexture(diffuseMap[0]);
    else{
        pm.SetTexture({DEFAULT_TEXTURE_WHITE_NAME,DEFAULT_TEX_TRANSFORM});
    }

    mm->AddMesh(pm);
}

std::vector<GRCore::TextureData> ModelImporter::ProcessTexture(aiMaterial *mat, aiTextureType type,
                                                               std::string typeName) {
    std::vector<TextureData> td;
    for(unsigned int i=0;i<mat->GetTextureCount(type);++i){
        std::cout<<"start load texture"<<std::endl;
        aiString str;
        mat->GetTexture(type,i,&str);
        std::string fileName(str.C_Str());
        fileName=fileName.substr(0,fileName.find_last_of('.'));
        std::cout<<"load texture:"<<fileName<<std::endl;
        fileName=fileName+typeName;
        bool skip=D3DSourceManager::GetSourceManager()->GetSource(fileName).type!=GR_NONE;
        if(!skip){
            D3DSourceManager::GetSourceManager()->LoadTexture(fileName,mDirPath+"/"+str.C_Str());
        }

        TextureData data;
        data.tex=fileName;
        data.texTrans=DEFAULT_TEX_TRANSFORM;
        td.push_back(data);
    }
    return td;
}