//
// Created by glodxy on 2019/12/23.
//

#ifndef GAME_MODELMESH_H
#define GAME_MODELMESH_H

#include "Mesh.h"
#include <map>

namespace GRCore {
    class ModelMesh: public BaseComponent{
    public:
        ModelMesh(){}
        void LoadBonesAndMapping(std::vector<BoneMatrix> bones,std::map<std::string,int> mapping);
        void SetRootName(std::string name);
        void AddNodeTransform(std::string nodeName,XMFLOAT4X4 trans);
        void AddNode(std::string root,std::vector<std::string> children);
        void AddMesh(Mesh m);
        void AddMapping(std::string name,int idx);
        void AddBoneMatrix(BoneMatrix m);

        void SetMaterialName(std::string material){
            for(auto mesh:mMeshes){
                mesh.SetMaterialName(material);
            }
        }

        std::string GetRootNode()const{
            return mRootNode;
        }

        std::vector<std::string> GetNodeChildren(std::string node){
            std::vector<std::string> result;
            if(mBoneNodes.find(node)==mBoneNodes.end()){
                return result;
            }
            return mBoneNodes[node];
        }

        void SetBoneMatrix(XMFLOAT4X4 matrix,int idx);

        XMFLOAT4X4 GetNodeTransform(std::string name){
            return mNodeTransform[name];
        }

        void PrintNode(){
            std::cout<<"start print modelmesh:"<<std::endl;
            PrintChildNode(mRootNode,0);
            std::cout<<"print finished"<<std::endl;
        }

        void PrintChildNode(std::string name,int level){
            if(mBoneNodes.find(name)==mBoneNodes.end()){
                return;
            }
            std::vector<std::string> names=mBoneNodes[name];
            std::string result;
            for(int i=0;i<level;++i){
                result+="-----";
            }
            for(auto iter:names){
                std::cout<<result<<iter<<std::endl;
                PrintChildNode(iter,level+1);
            }
        }

        int GetBoneIdx(std::string name){
            if(mBoneMapping.find(name)==mBoneMapping.end()){
                return -1;
            }
            return mBoneMapping[name];
        }

        int GetBoneSize()const{
            return mBones.size();
        }

        std::vector<Mesh> GetMeshes()const{
            return mMeshes;
        }

        std::string getName()const{
            return "ModelMesh";
        }



       std::vector<XMFLOAT4X4> GetBones(){
            std::vector<XMFLOAT4X4> bones;
            for(int i=0;i<mBones.size();++i){
                bones.push_back(mBones[i].finalWorld);
            }
            return bones;
        }

        XMFLOAT4X4 GlobalTransform;
    protected:
        std::map<std::string,int> mBoneMapping;
        std::vector<BoneMatrix> mBones;
        std::vector<Mesh> mMeshes;
        std::string mRootNode;
        std::map<std::string,std::vector<std::string>> mBoneNodes;
        std::map<std::string,XMFLOAT4X4> mNodeTransform;
    };
}


#endif //GAME_MODELMESH_H
