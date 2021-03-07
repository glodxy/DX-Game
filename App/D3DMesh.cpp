//
// Created by 田淙宇 on 2019/12/2.
//

#include "D3DMesh.h"

using namespace GRCore;

D3DMesh::D3DMesh(ID3D11Device *device, ID3D11DeviceContext *context) {
    mDevice=device;
    mContext=context;
}


D3DMesh::~D3DMesh() {

}

void D3DMesh::Reset() {
    ReleaseCom(mVertexBuffer);
    ReleaseCom(mIndexBuffer);
    ReleaseCom(mNormalBuffer);
    ReleaseCom(mUvBuffer);
}

bool D3DMesh::Initialize(Vertex*v,int vCount,Index*i,int iCount) {
    mVertexCount=vCount;
    mIndexCount=iCount;

    //第一,填充(顶点)缓存形容结构体和子资源数据结构体,并创建顶点缓存
    D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * mVertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = v;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;
    ThrowIfFailed(mDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer));

    //第二,填充(索引)缓存形容结构体和子资源数据结构体,并创建索引缓存
    D3D11_BUFFER_DESC  indexBufferDesc;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(WORD) * mIndexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = i;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;
    ThrowIfFailed(mDevice->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer));

    return true;
}

bool D3DMesh::SetNormalBuffer(GRCore::Normal *n, int ncount) {
    mNormalCount=ncount;

    D3D11_BUFFER_DESC normalBufferDesc;
    normalBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    normalBufferDesc.ByteWidth = sizeof(Normal) * mNormalCount;
    normalBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    normalBufferDesc.CPUAccessFlags = 0;
    normalBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA normalData;
    normalData.pSysMem = n;
    normalData.SysMemPitch = 0;
    normalData.SysMemSlicePitch = 0;
    ThrowIfFailed(mDevice->CreateBuffer(&normalBufferDesc, &normalData, &mNormalBuffer));
}

bool D3DMesh::SetUVBuffer(GRCore::UV *u, int uvcount) {
    mUvCount=uvcount;

    D3D11_BUFFER_DESC uvBufferDesc;
    uvBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    uvBufferDesc.ByteWidth = sizeof(UV) * mUvCount;
    uvBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    uvBufferDesc.CPUAccessFlags = 0;
    uvBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA uvData;
    uvData.pSysMem = u;
    uvData.SysMemPitch = 0;
    uvData.SysMemSlicePitch = 0;
    ThrowIfFailed(mDevice->CreateBuffer(&uvBufferDesc, &uvData, &mUvBuffer));
}

bool D3DMesh::SetBoneBuffer(GRCore::BoneData *bone, int count) {
    mBoneCount=count;

    D3D11_BUFFER_DESC boneBufferDesc;
    boneBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    boneBufferDesc.ByteWidth = sizeof(BoneData) * mBoneCount;
    boneBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    boneBufferDesc.CPUAccessFlags = 0;
    boneBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA boneData;
    boneData.pSysMem = bone;
    boneData.SysMemPitch = 0;
    boneData.SysMemSlicePitch = 0;
    ThrowIfFailed(mDevice->CreateBuffer(&boneBufferDesc, &boneData, &mBoneBuffer));
}

void D3DMesh::Render(D3DShader*s,std::vector<ID3D11ShaderResourceView*> texture,std::vector<XMFLOAT4X4> texTrans,MATERIAL_VALUE* m,ID3D11ShaderResourceView* skybox) {
    std::vector<ID3D11ShaderResourceView*> tempTextureArray;
    std::cout<<"texture size:"<<texture.size()<<std::endl;
    for(auto iter:texture){
        tempTextureArray.push_back(iter);
    }
    XMFLOAT4X4* pTexTrans=&(texTrans[0]);


    UINT stride = sizeof(Vertex); //每个顶点元素的跨度大小，或者说每个顶点元素的大小
    UINT offset = 0;
    mContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
    if(mNormalBuffer!= nullptr&&s->ContainsInput("NORMAL")){
        stride=sizeof(Normal);
        offset=0;
        mContext->IASetVertexBuffers(1,1,&mNormalBuffer,&stride,&offset);
    }
    if(mUvBuffer!= nullptr&&s->ContainsInput("TEXCOORD")){
        stride= sizeof(UV);
        offset=0;
        mContext->IASetVertexBuffers(2,1,&mUvBuffer,&stride,&offset);
    }


    //设置索引缓存
    mContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0); //Word为两个字节

    //设置拓扑方式
    mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    if(s->IsEffect()){
        s->SetFloat("g_time",GR_TIMER->TotalTime());
        s->SetMatrix4("Model",GR_MODEL_MATRIX);
        s->SetMatrix4("View",GR_VIEW_MATRIX);
        s->SetMatrix4("Projection",GR_PROJ_MATRIX);
        s->SetMatrix4("g_modelTexTransform",GR_MODEL_TEX_TRANSFORM_MATRIX);
        s->SetVector("g_ambientLight",GR_AMBIENT_LIGHT);
        s->SetStruct("g_lights",GR_LIGHTS,30);
        s->SetInt("g_dirLightSize",GR_DIRLIGHT_SIZE);
        s->SetInt("g_pointLightSize",GR_POINTLIGHT_SIZE);
        s->SetInt("g_spotLightSize",GR_SPOTLIGHT_SIZE);
        s->SetInt("g_texSize",texture.size());
        s->SetStruct("material",m);
        s->SetTexture("g_SkyBox",skybox);
        s->SetVector("CameraPos",GR_CAMERA_POS);
        s->SetMatrix4("NormalToW",GR_NORMAL_WORLD_MATRIX);
        s->SetTextureArray("g_tex",&tempTextureArray[0],texture.size());
        s->SetMatrix4Array("g_texTransform",pTexTrans,texture.size());
        int num=s->GetPassNum();
        std::cout<<"shader pass num:"<<num<<std::endl;
        for(int i=0;i<num;++i){
            s->Use(i);
            mContext->DrawIndexed(mIndexCount,0,0);
        }
    }else {
        std::cerr<<"this is a shader, cannot to be rendered"<<std::endl;
    }

}

void D3DMesh::Render(GRCore::D3DShader *s, std::vector<ID3D11ShaderResourceView *> texture,
                     std::vector<XMFLOAT4X4> texTrans, GRCore::MATERIAL_VALUE *m, XMFLOAT4X4 *bones,bool useBones,
                     ID3D11ShaderResourceView *skybox) {
    std::vector<ID3D11ShaderResourceView*> tempTextureArray;
    std::cout<<"texture size:"<<texture.size()<<std::endl;
    for(auto iter:texture){
        tempTextureArray.push_back(iter);
    }
    XMFLOAT4X4* pTexTrans=&(texTrans[0]);


    UINT stride = sizeof(Vertex); //每个顶点元素的跨度大小，或者说每个顶点元素的大小
    UINT offset = 0;
    mContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
    if(mNormalBuffer!= nullptr&&s->ContainsInput("NORMAL")){
        stride=sizeof(Normal);
        offset=0;
        mContext->IASetVertexBuffers(1,1,&mNormalBuffer,&stride,&offset);
    }
    if(mUvBuffer!= nullptr&&s->ContainsInput("TEXCOORD")){
        stride= sizeof(UV);
        offset=0;
        mContext->IASetVertexBuffers(2,1,&mUvBuffer,&stride,&offset);
    }
    if(mBoneBuffer!=nullptr){
        std::cout<<"setup bones"<<std::endl;
        stride= sizeof(BoneData);
        offset=0;
        mContext->IASetVertexBuffers(3,1,&mBoneBuffer,&stride,&offset);
    }

    //设置索引缓存
    mContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0); //Word为两个字节

    //设置拓扑方式
    mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    if(s->IsEffect()){
        s->SetFloat("g_time",GR_TIMER->TotalTime());
        s->SetMatrix4("Model",GR_MODEL_MATRIX);
        s->SetMatrix4("View",GR_VIEW_MATRIX);
        s->SetMatrix4("Projection",GR_PROJ_MATRIX);
        s->SetMatrix4("g_modelTexTransform",GR_MODEL_TEX_TRANSFORM_MATRIX);
        s->SetMatrix4Array("Bones",bones,MAX_BONES);
        s->SetVector("g_ambientLight",GR_AMBIENT_LIGHT);
        s->SetStruct("g_lights",GR_LIGHTS,30);
        s->SetInt("g_dirLightSize",GR_DIRLIGHT_SIZE);
        s->SetInt("g_pointLightSize",GR_POINTLIGHT_SIZE);
        s->SetInt("g_spotLightSize",GR_SPOTLIGHT_SIZE);
        s->SetInt("g_texSize",texture.size());
        s->SetStruct("material",m);
        s->SetTexture("g_SkyBox",skybox);
        s->SetVector("CameraPos",GR_CAMERA_POS);
        s->SetMatrix4("NormalToW",GR_NORMAL_WORLD_MATRIX);
        s->SetTextureArray("g_tex",&tempTextureArray[0],texture.size());
        s->SetMatrix4Array("g_texTransform",pTexTrans,texture.size());
        if(useBones){
            s->SetInt("UseBones",1);
        }else{
            s->SetInt("UseBones",0);
        }
        int num=s->GetPassNum();
        std::cout<<"shader pass num:"<<num<<std::endl;
        for(int i=0;i<num;++i){
            s->Use(i);
            mContext->DrawIndexed(mIndexCount,0,0);
        }
    }else {
        std::cerr<<"this is a shader, cannot to be rendered"<<std::endl;
    }
}