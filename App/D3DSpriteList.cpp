//
// Created by 田淙宇 on 2019/12/13.
//

#include "D3DSpriteList.h"

using namespace GRCore;
D3DSpriteList::D3DSpriteList(ID3D11Device *device, ID3D11DeviceContext *context) {
    mDevice=device;
    mContext=context;
}

void D3DSpriteList::Reset() {
    mVertexBuffer.Reset();
}

void D3DSpriteList::Initialize(GRCore::SPRITE_VALUE *value, int count, D3DTexture* texture, D3DShader* shader, MATERIAL_VALUE *material) {
    mCount=count;
    D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(SPRITE_VALUE) * mCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = value;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;
    ThrowIfFailed(mDevice->CreateBuffer(&vertexBufferDesc, &vertexData, mVertexBuffer.GetAddressOf()));

    mTexture=texture;
    mShader=shader;
    mMaterial=material;
}

void D3DSpriteList::Render() {
    std::vector<ID3D11ShaderResourceView*> tempTextureArray;
    tempTextureArray.push_back(mTexture->GetTexture());
    XMFLOAT4X4* pTexTrans=&DEFAULT_TEX_TRANSFORM;


    UINT stride = sizeof(SPRITE_VALUE); //每个顶点元素的跨度大小，或者说每个顶点元素的大小
    UINT offset = 0;
    mContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);

    //设置拓扑方式
    mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
    if(mShader->IsEffect()){
        mShader->SetMatrix4("Model",GR_MODEL_MATRIX);
        mShader->SetMatrix4("View",GR_VIEW_MATRIX);
        mShader->SetMatrix4("Projection",GR_PROJ_MATRIX);
        mShader->SetVector("g_ambientLight",GR_AMBIENT_LIGHT);
        mShader->SetStruct("g_lights",GR_LIGHTS,30);
        mShader->SetInt("g_dirLightSize",GR_DIRLIGHT_SIZE);
        mShader->SetInt("g_pointLightSize",GR_POINTLIGHT_SIZE);
        mShader->SetInt("g_spotLightSize",GR_SPOTLIGHT_SIZE);
        mShader->SetStruct("material",mMaterial);
        mShader->SetInt("g_texSize",1);

        mShader->SetVector("CameraPos",GR_CAMERA_POS);
        mShader->SetMatrix4("NormalToW",GR_NORMAL_WORLD_MATRIX);
        mShader->SetTextureArray("g_tex",&tempTextureArray[0],1);
        mShader->SetMatrix4Array("g_texTransform",pTexTrans,1);
        int num=mShader->GetPassNum();
        for(int i=0;i<num;++i){
            mShader->Use(i);
            mContext->Draw(mCount,0);
        }
    }else {
        std::cerr<<"this is a shader, cannot to be rendered"<<std::endl;
    }
}