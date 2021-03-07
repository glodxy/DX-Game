//
// Created by 田淙宇 on 2019/12/7.
//

#include "D3DTexture.h"
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>

using namespace GRCore;

D3DTexture::D3DTexture(ID3D11Device *device,ID3D11DeviceContext*context) {
    mDevice=device;
    mContext=context;
}

D3DTexture::D3DTexture(ID3D11ShaderResourceView *resource) {
    mResource.Attach(resource);
}

void D3DTexture::Reset() {
    mResource.Reset();
}

bool D3DTexture::Initialize(const std::string &fileName,bool isCube,bool generateMips) {
    std::string fileType=fileName.substr(fileName.find_last_of('.')+1);
    std::cout<<"texture type:"<<fileType<<std::endl;
    if(!isCube) {
        if (fileType == "dds") {
            return InitDDSTexture(&(AnsiToWString(fileName)[0]));
        } else {
            if(fileType=="tga"||fileType=="TGA"){
                return InitTGATexture(&(AnsiToWString(fileName)[0]));
            }else {
                return InitWICTexture(&(AnsiToWString(fileName)[0]));
            }
        }
    }else{
        return InitWICTextureCube(&(AnsiToWString(fileName)[0]),generateMips);
    }
}

bool D3DTexture::InitDDSTexture(WCHAR *fileName) {

    ThrowIfFailed(CreateDDSTextureFromFile(mDevice,fileName,nullptr,mResource.GetAddressOf()));
    return true;
}

bool D3DTexture::InitWICTexture(WCHAR *fileName) {
    std::cout<<"init wic texture:"<<WStringToAnsi(fileName)<<std::endl;
    ThrowIfFailed(CreateWICTextureFromFile(mDevice,fileName,nullptr,mResource.GetAddressOf()));
    return true;
}

bool D3DTexture::InitTGATexture(WCHAR *fileName) {
    ScratchImage image;
    ThrowIfFailed(LoadFromTGAFile(fileName,nullptr,image));
    ThrowIfFailed(CreateShaderResourceView(mDevice,image.GetImages(),image.GetImageCount(),image.GetMetadata(),mResource.GetAddressOf()));
}

bool D3DTexture::InitWICTextureCube(WCHAR *fileName,bool mipmap) {
    McComPtr<ID3D11Texture2D> srcTex;
    McComPtr <ID3D11ShaderResourceView> srcTexSRV;

    ThrowIfFailed(CreateWICTextureFromFileEx(mDevice,
            mContext,fileName,0,D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE | (mipmap ? D3D11_BIND_RENDER_TARGET : 0),
            0,(mipmap ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0),
            WIC_LOADER_DEFAULT,(ID3D11Resource**)srcTex.GetAddressOf(),
            (mipmap ? srcTexSRV.GetAddressOf() : nullptr)));


    D3D11_TEXTURE2D_DESC texDesc;
    srcTex->GetDesc(&texDesc);
    UINT squareLength = texDesc.Width / 4;
    D3D11_TEXTURE2D_DESC texArrayDesc;
    texArrayDesc.Width = squareLength;
    texArrayDesc.Height = squareLength;
    texArrayDesc.MipLevels = (mipmap ? texDesc.MipLevels - 2 : 1);    // 立方体的mip等级比整张位图的少2
    texArrayDesc.ArraySize = 6;
    texArrayDesc.Format = texDesc.Format;
    texArrayDesc.SampleDesc.Count = 1;
    texArrayDesc.SampleDesc.Quality = 0;
    texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
    texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texArrayDesc.CPUAccessFlags = 0;
    texArrayDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;   // 允许从中创建TextureCube

    McComPtr <ID3D11Texture2D> texArray;
    ThrowIfFailed(mDevice->CreateTexture2D(&texArrayDesc,nullptr,texArray.GetAddressOf()));

    D3D11_BOX box;
    box.front=0;
    box.back=1;

    for (UINT i = 0; i < texArrayDesc.MipLevels; ++i)
    {
        // +X面拷贝
        box.left = squareLength * 2;
        box.top = squareLength;
        box.right = squareLength * 3;
        box.bottom = squareLength * 2;
        mContext->CopySubresourceRegion(
                texArray.Get(),
                D3D11CalcSubresource(i, D3D11_TEXTURECUBE_FACE_POSITIVE_X, texArrayDesc.MipLevels),
                0, 0, 0,
                srcTex.Get(),
                i,
                &box);

        // -X面拷贝
        box.left = 0;
        box.top = squareLength;
        box.right = squareLength;
        box.bottom = squareLength * 2;
        mContext->CopySubresourceRegion(
                texArray.Get(),
                D3D11CalcSubresource(i, D3D11_TEXTURECUBE_FACE_NEGATIVE_X, texArrayDesc.MipLevels),
                0, 0, 0,
                srcTex.Get(),
                i,
                &box);

        // +Y面拷贝
        box.left = squareLength;
        box.top = 0;
        box.right = squareLength * 2;
        box.bottom = squareLength;
        mContext->CopySubresourceRegion(
                texArray.Get(),
                D3D11CalcSubresource(i, D3D11_TEXTURECUBE_FACE_POSITIVE_Y, texArrayDesc.MipLevels),
                0, 0, 0,
                srcTex.Get(),
                i,
                &box);


        // -Y面拷贝
        box.left = squareLength;
        box.top = squareLength * 2;
        box.right = squareLength * 2;
        box.bottom = squareLength * 3;
        mContext->CopySubresourceRegion(
                texArray.Get(),
                D3D11CalcSubresource(i, D3D11_TEXTURECUBE_FACE_NEGATIVE_Y, texArrayDesc.MipLevels),
                0, 0, 0,
                srcTex.Get(),
                i,
                &box);

        // +Z面拷贝
        box.left = squareLength;
        box.top = squareLength;
        box.right = squareLength * 2;
        box.bottom = squareLength * 2;
        mContext->CopySubresourceRegion(
                texArray.Get(),
                D3D11CalcSubresource(i, D3D11_TEXTURECUBE_FACE_POSITIVE_Z, texArrayDesc.MipLevels),
                0, 0, 0,
                srcTex.Get(),
                i,
                &box);

        // -Z面拷贝
        box.left = squareLength * 3;
        box.top = squareLength;
        box.right = squareLength * 4;
        box.bottom = squareLength * 2;
        mContext->CopySubresourceRegion(
                texArray.Get(),
                D3D11CalcSubresource(i, D3D11_TEXTURECUBE_FACE_NEGATIVE_Z, texArrayDesc.MipLevels),
                0, 0, 0,
                srcTex.Get(),
                i,
                &box);

        // 下一个mipLevel的纹理宽高都是原来的1/2
        squareLength /= 2;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format=texArrayDesc.Format;
    viewDesc.ViewDimension=D3D11_SRV_DIMENSION_TEXTURECUBE;
    viewDesc.TextureCube.MostDetailedMip=0;
    viewDesc.TextureCube.MipLevels=texArrayDesc.MipLevels;
    ThrowIfFailed(mDevice->CreateShaderResourceView(texArray.Get(),&viewDesc,mResource.GetAddressOf()));

    texArray.Reset();
    srcTex.Reset();
    srcTexSRV.Reset();

    return true;
}



