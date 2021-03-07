//
// Created by 田淙宇 on 2019/12/16.
//

#ifndef GAME_SPRITE_H
#define GAME_SPRITE_H

#include "BaseComponent.h"

namespace GRCore {
    class Sprite:public BaseComponent{
    public:
        Sprite(const std::string&tex);
        ~Sprite()=default;

        void SetShader(const std::string&shader){
            mShader=shader;
        }
        void SetTexture(TextureData tex){
            mTex=tex;
        }
        std::string GetShaderName()const{
            return mShader;
        }

        std::string GetTextureName()const{
            return mTex.tex;
        }

        XMFLOAT4X4 GetTextureTranform()const{
            return mTex.texTrans;
        }

        std::string getName()const{
            return "Sprite";
        }

    protected:
        TextureData mTex;
        std::string mShader;
    };
}


#endif //GAME_SPRITE_H
