//
// Created by 田淙宇 on 2019/12/16.
//

#ifndef GAME_D3DSPRITE_H
#define GAME_D3DSPRITE_H

#include "D3DUtil.h"
#include "D3DShader.h"

namespace GRCore {
    class D3DSprite {
    public:
        D3DSprite(ID3D11Device* device,ID3D11DeviceContext* context);
        void Initialize(const SPRITE_VALUE* value);
        void Render(D3DShader* s,ID3D11ShaderResourceView* texture,XMFLOAT4X4 texTrans);

        void Reset();
    private:
        ID3D11Device* mDevice;
        ID3D11DeviceContext* mContext;
        McComPtr <ID3D11Buffer> mVertexBuffer;
    };
}


#endif //GAME_D3DSPRITE_H
