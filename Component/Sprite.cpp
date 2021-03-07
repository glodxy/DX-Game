//
// Created by 田淙宇 on 2019/12/16.
//

#include "Sprite.h"
using namespace GRCore;

Sprite::Sprite(const std::string &tex):BaseComponent(){
    mTex.tex=tex;
    mTex.texTrans=DEFAULT_TEX_TRANSFORM;
}