//
// Created by 田淙宇 on 2019/12/7.
//

#ifndef GAME_D3DTEXTURE_H
#define GAME_D3DTEXTURE_H

#include "D3DUtil.h"

namespace GRCore {
    class D3DTexture {
    public:
        explicit D3DTexture(ID3D11Device* device,ID3D11DeviceContext*context);
        explicit D3DTexture(ID3D11ShaderResourceView* resource);

        bool Initialize(const std::string& fileName,bool isCube=false,bool generateMips=false);
        ID3D11ShaderResourceView* GetTexture(){
            return mResource.Get();
        }
        void Reset();
    protected:
        bool InitDDSTexture(WCHAR* fileName);
        bool InitWICTexture(WCHAR* fileName);
        bool InitWICTextureCube(WCHAR* fileName,bool mipmap);
        bool InitTGATexture(WCHAR* fileName);

        ID3D11Device* mDevice;
        ID3D11DeviceContext* mContext;
        McComPtr <ID3D11ShaderResourceView> mResource;
    };
}


#endif //GAME_D3DTEXTURE_H
