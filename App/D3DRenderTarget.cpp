//
// Created by 田淙宇 on 2019/12/15.
//

#include "D3DRenderTarget.h"

using namespace GRCore;

D3DRenderTarget::D3DRenderTarget(ID3D11Device *device, ID3D11DeviceContext *context) {
    mDevice=device;
    mContext=context;
}

void D3DRenderTarget::Reset() {
    mShaderResource.Reset();
    mRTV.Reset();
    mDSV.Reset();
}

void D3DRenderTarget::Initialize(int width,int height) {

    mHeight=height;
    mWidth=width;
    D3D11_TEXTURE2D_DESC desc;
    desc.Width=width;
    desc.Height=height;
    desc.MipLevels=1;
    desc.ArraySize=1;
    desc.SampleDesc.Count=4;
    desc.SampleDesc.Quality=GR_MSAA_QUALITY-1;
    desc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.Usage=D3D11_USAGE_DEFAULT;
    desc.BindFlags=D3D11_BIND_RENDER_TARGET|D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags=0;
    desc.MiscFlags=0;



    ThrowIfFailed(mDevice->CreateTexture2D(&desc,nullptr,mTexture.GetAddressOf()));


    ThrowIfFailed(mDevice->CreateRenderTargetView(mTexture.Get(),NULL,mRTV.GetAddressOf()));



    ThrowIfFailed(mDevice->CreateShaderResourceView(mTexture.Get(),NULL,mShaderResource.GetAddressOf()));


    //创建深度和模板视图
    desc.SampleDesc.Count = 4;
    desc.SampleDesc.Quality = GR_MSAA_QUALITY-1;
    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    McComPtr<ID3D11Texture2D> depthTex;
    ThrowIfFailed(mDevice->CreateTexture2D(&desc, nullptr, depthTex.GetAddressOf()));



    ThrowIfFailed(mDevice->CreateDepthStencilView(depthTex.Get(),NULL,mDSV.GetAddressOf()));
    mTexture.Reset();
}

void D3DRenderTarget::Initialize(const McComPtr<ID3D11Texture2D>& tex) {
    mTexture=tex;
    ThrowIfFailed(mDevice->CreateRenderTargetView(tex.Get(),0,mRTV.GetAddressOf()));


    //创建深度和模板视图
    D3D11_TEXTURE2D_DESC dtDesc;
    dtDesc.Width=GR_WIDTH;
    dtDesc.Height = GR_HEIGHT;
    dtDesc.MipLevels = 1;
    dtDesc.ArraySize = 1;
    dtDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    //多重采样
    dtDesc.SampleDesc.Count = 4;
    dtDesc.SampleDesc.Quality =GR_MSAA_QUALITY-1;
    dtDesc.Usage = D3D11_USAGE_DEFAULT;
    dtDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    dtDesc.CPUAccessFlags = 0;
    dtDesc.MiscFlags = 0;
    McComPtr<ID3D11Texture2D> depthTex;
    ThrowIfFailed(mDevice->CreateTexture2D(&dtDesc,//要创建的纹理的形容
                                   nullptr,
                                   depthTex.GetAddressOf()));


    ThrowIfFailed(mDevice->CreateDepthStencilView(depthTex.Get(),NULL,mDSV.GetAddressOf()));
    mTexture.Reset();
}

McComPtr<ID3D11DepthStencilView> D3DRenderTarget::GteDSV() {
    return mDSV;
}

McComPtr<ID3D11RenderTargetView> D3DRenderTarget::GetRTV() {
    return mRTV;
}

McComPtr<ID3D11ShaderResourceView> D3DRenderTarget::GetResource() {
    return mShaderResource;
}

void D3DRenderTarget::Clear() {
    float c[]={0.0f,0.0f,0.0f,1.0f};
    mContext->ClearRenderTargetView(mRTV.Get(),c);
    mContext->ClearDepthStencilView(mDSV.Get(),D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL,1.0f,0);
}