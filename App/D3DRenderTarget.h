//
// Created by 田淙宇 on 2019/12/15.
//

#ifndef GAME_D3DRENDERTARGET_H
#define GAME_D3DRENDERTARGET_H

#include "D3DUtil.h"

namespace GRCore {
    class D3DRenderTarget {
    public:
        D3DRenderTarget(ID3D11Device* device,ID3D11DeviceContext* context);
        void Reset();

        void Initialize(int width=GR_WIDTH,int height=GR_HEIGHT);
        void Initialize(const McComPtr<ID3D11Texture2D>& tex);
        McComPtr<ID3D11RenderTargetView> GetRTV();
        McComPtr<ID3D11DepthStencilView> GteDSV();
        McComPtr<ID3D11ShaderResourceView> GetResource();

        int GetWidth()const{
            return mWidth;
        }
        int GetHeight()const{
            return mHeight;
        }
        void Clear();
    private:
        ID3D11Device* mDevice;
        ID3D11DeviceContext* mContext;
        int mWidth,mHeight;
        McComPtr <ID3D11Texture2D> mTexture;
        McComPtr <ID3D11ShaderResourceView> mShaderResource;
        McComPtr<ID3D11RenderTargetView> mRTV;
        McComPtr<ID3D11DepthStencilView> mDSV;
    };
}


#endif //GAME_D3DRENDERTARGET_H
