//
// Created by 田淙宇 on 2019/12/13.
//

#ifndef GAME_D3DSPRITELIST_H
#define GAME_D3DSPRITELIST_H
#include "D3DUtil.h"
#include "../Math/DXMath.h"
#include "D3DShader.h"
#include "D3DTexture.h"


namespace GRCore {
    class D3DSpriteList {
    public:
        D3DSpriteList(ID3D11Device *device, ID3D11DeviceContext *context);
        D3DSpriteList(const D3DSpriteList&)=delete;

        void Reset();
        ~D3DSpriteList()=default;

    public:
        void Initialize(SPRITE_VALUE* value,int count,D3DTexture* texture,D3DShader* shader,MATERIAL_VALUE* material);
        void Render();
    private:
        ID3D11Device* mDevice;
        ID3D11DeviceContext* mContext;
        McComPtr <ID3D11Buffer> mVertexBuffer;
        int mCount;
        D3DTexture* mTexture;
        D3DShader* mShader;
        MATERIAL_VALUE* mMaterial;
    };
}


#endif //GAME_D3DSPRITELIST_H
