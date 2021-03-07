//
// Created by 田淙宇 on 2019/12/13.
//

#include "SpriteList.h"

using namespace GRCore;

SpriteList::SpriteList(std::string tex) {
    mShader=DEFAULT_SPRITE_SHADER_NAME;
    mTexName=tex;
    mMaterial=DEFAULT_MATERIAL_NAME;
}

SpriteList::SpriteList(float* pos,float*size,int count,std::string tex) {
    mTexName=tex;
    for(int i=0;i<count;++i){
        mPosition.push_back(XMFLOAT3(pos[3*i],pos[3*i+1],pos[3*i+2]));
        mSize.push_back(XMFLOAT2(size[2*i],size[2*i+1]));
    }
    mShader=DEFAULT_SPRITE_SHADER_NAME;
    mMaterial=DEFAULT_MATERIAL_NAME;
}

void SpriteList::AddItem(XMFLOAT3 position, XMFLOAT2 size) {
    mPosition.push_back(position);
    mSize.push_back(size);
}

void SpriteList::SetShader(std::string shader) {
    mShader=shader;
}

void SpriteList::SetMaterial(std::string material) {
    mMaterial=material;
}

void SpriteList::update() {

}