//
// Created by 田淙宇 on 2019/12/16.
//

#include "D3DSprite.h"

using namespace GRCore;

D3DSprite::D3DSprite(ID3D11Device *device, ID3D11DeviceContext *context) {
    mDevice=device;
    mContext=context;
}

void D3DSprite::Reset() {
    mVertexBuffer.Reset();
}

void D3DSprite::Initialize(const GRCore::SPRITE_VALUE* value) {
    D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(SPRITE_VALUE);
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = value;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;
    ThrowIfFailed(mDevice->CreateBuffer(&vertexBufferDesc, &vertexData, mVertexBuffer.GetAddressOf()));
}

void D3DSprite::Render(GRCore::D3DShader *s, ID3D11ShaderResourceView *texture,
                       XMFLOAT4X4 texTrans) {

    ID3D11ShaderResourceView** pTex=&texture;
    XMFLOAT4X4* pTexTrans=&texTrans;
    UINT stride = sizeof(SPRITE_VALUE); //每个顶点元素的跨度大小，或者说每个顶点元素的大小
    UINT offset = 0;
    mContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
    //设置拓扑方式
    mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
    if(s->IsEffect()){
        s->SetFloat("g_time",GR_TIMER->TotalTime());
        s->SetMatrix4("Model",GR_MODEL_MATRIX);
        s->SetMatrix4("View",GR_VIEW_MATRIX);
        s->SetMatrix4("Projection",GR_PROJ_MATRIX);
        s->SetVector("g_ambientLight",GR_AMBIENT_LIGHT);
        s->SetMatrix4("g_modelTexTransform",GR_MODEL_TEX_TRANSFORM_MATRIX);
        s->SetStruct("g_lights",GR_LIGHTS,30);
        s->SetInt("g_dirLightSize",GR_DIRLIGHT_SIZE);
        s->SetInt("g_pointLightSize",GR_POINTLIGHT_SIZE);
        s->SetInt("g_spotLightSize",GR_SPOTLIGHT_SIZE);
        s->SetInt("g_texSize",1);

        s->SetVector("CameraPos",GR_CAMERA_POS);
        s->SetMatrix4("NormalToW",GR_NORMAL_WORLD_MATRIX);
        s->SetTextureArray("g_tex",pTex,1);
        s->SetMatrix4Array("g_texTransform",pTexTrans,1);
        int num=s->GetPassNum();
        for(int i=0;i<num;++i){
            s->Use(i);
            mContext->Draw(1,0);
        }
    }else {
        std::cerr<<"this is a shader, cannot to be rendered"<<std::endl;
    }
}