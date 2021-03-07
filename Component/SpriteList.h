//
// Created by 田淙宇 on 2019/12/13.
//

#ifndef GAME_SPRITELIST_H
#define GAME_SPRITELIST_H

#include "BaseComponent.h"

namespace GRCore {
    class SpriteList: public BaseComponent{
    public:
        SpriteList(std::string tex);
        SpriteList(float* pos,float* size,int count,std::string tex);
        void SetShader(std::string shader);
        void SetMaterial(std::string material);
        SpriteList(const SpriteList&)=delete;
        ~SpriteList()=default;

        void update();


        void AddItem(XMFLOAT3 position,XMFLOAT2 size);
        std::string getName()const{
            return "SpriteList";
        }

        std::vector<XMFLOAT3> GetPositions()const{
            return mPosition;
        }

        std::vector<XMFLOAT2> GetSizes()const{
            return mSize;
        }

        std::string GetShaderName()const{
            return mShader;
        }

        std::string GetTextureName()const{
            return mTexName;
        }

        std::string GetMaterialName()const{
            return mMaterial;
        }
    protected:
        std::vector<XMFLOAT3> mPosition;
        std::vector<XMFLOAT2> mSize;
        std::string mShader;
        std::string mTexName;
        std::string mMaterial;
    };
}


#endif //GAME_SPRITELIST_H
