//
// Created by 田淙宇 on 2019/12/9.
//

#include "GeometryGenerator.h"

using namespace GRCore;
MeshData GeometryGenerator::CreateGrid(float width, float depth, UINT32 m, UINT32 n) {
    MeshData meshData;

    UINT32 vertexCount=m*n;
    UINT32 faceCount=(m-1)*(n-1)*2;

    float halfWidth=.5f*width;
    float halfDepth=.5f*depth;

    float dx=width/(m-1);
    float dz=depth/(n-1);
    float du=1.0f/(m-1);
    float dv=1.0f/(n-1);

    meshData.v=new Vertex[vertexCount];
    meshData.vertex_count=vertexCount;
    meshData.u=new UV[vertexCount];
    meshData.uv_count=vertexCount;
    meshData.n=new Normal[vertexCount];
    meshData.normal_count=vertexCount;

    //生成顶点数据
    for(UINT32 i=0;i<n;++i){
        float z=halfDepth-i*dz;
        for(UINT32 j=0;j<m;++j){
            float x=-halfWidth+j*dx;
            meshData.v[i*m+j]=XMFLOAT3(x,.0f,z);
            meshData.n[i*m+j]=XMFLOAT3(.0f,1.0f,.0f);
            meshData.u[i*m+j]=XMFLOAT2(j*du,i*dv);
        }
    }

    //生成索引数据
    meshData.i=new Index[faceCount*3];
    meshData.index_count=faceCount*3;
    UINT32 k=0;
    for(UINT32 i=0;i<n-1;++i){
        for(UINT32 j=0;j<m-1;++j){
            meshData.i[k]=i*m+j;
            meshData.i[k+1]=i*m+j+1;
            meshData.i[k+2]=(i+1)*m+j;

            meshData.i[k+3]=(i+1)*m+j;
            meshData.i[k+4]=i*m+j+1;
            meshData.i[k+5]=(i+1)*m+j+1;
            k+=6;
        }
    }
    return meshData;
}

MeshData GeometryGenerator::CreateSphere(float radius, UINT levels, UINT slices) {
    MeshData meshData;
    UINT vertexCount = 2 + (levels - 1) * (slices + 1);
    UINT indexCount = 6 * (levels - 1) * slices;

    meshData.v=new Vertex[vertexCount];
    meshData.vertex_count=vertexCount;
    meshData.i=new Index[indexCount];
    meshData.index_count=indexCount;
    meshData.u=new UV[vertexCount];
    meshData.uv_count=vertexCount;
    meshData.n=new Normal[vertexCount];
    meshData.normal_count=vertexCount;



    float phi = 0.0f, theta = 0.0f;
    float per_phi = XM_PI / levels;
    float per_theta = XM_2PI / slices;
    float x, y, z;

    int vertexIndex=0;

    // 放入顶端点
    Vertex v0=XMFLOAT3(.0f,radius,.0f);
    Normal n0=XMFLOAT3(.0F,1.0F,.0F);
    UV uv0=XMFLOAT2(0.0f,0.0f);
    meshData.v[vertexIndex]=v0;
    meshData.n[vertexIndex]=n0;
    meshData.u[vertexIndex]=uv0;
    vertexIndex++;
    for (UINT i = 1; i < levels; ++i)
    {
        phi = per_phi * i;
        // 需要slices + 1个顶点是因为 起点和终点需为同一点，但纹理坐标值不一致
        for (UINT j = 0; j <= slices; ++j)
        {
            theta = per_theta * j;
            x = radius * sinf(phi) * cosf(theta);
            y = radius * cosf(phi);
            z = radius * sinf(phi) * sinf(theta);
            // 计算出局部坐标、法向量、Tangent向量和纹理坐标
            XMFLOAT3 pos = XMFLOAT3(x, y, z);
            XMFLOAT3 normal;
            XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&pos)));

            UV uv=XMFLOAT2(theta / XM_2PI, phi / XM_PI);
            meshData.v[vertexIndex]=pos;
            meshData.n[vertexIndex]=normal;
            meshData.u[vertexIndex]=uv;
            vertexIndex++;
        }
    }


    // 放入底端点
    v0=XMFLOAT3(.0f,-radius,.0f);
    n0=XMFLOAT3(.0F,-1.0F,.0F);
    uv0=XMFLOAT2(0.0f,1.0f);
    meshData.v[vertexIndex]=v0;
    meshData.n[vertexIndex]=n0;
    meshData.u[vertexIndex]=uv0;
    vertexIndex++;


    int iIndex=0;
    // 逐渐放入索引
    if (levels > 1)
    {
        for (UINT j = 1; j <= slices; ++j)
        {
            meshData.i[iIndex++] = 0;
            meshData.i[iIndex++] = j % (slices + 1) + 1;
            meshData.i[iIndex++] = j;
        }
    }


    for (UINT i = 1; i < levels - 1; ++i)
    {
        for (UINT j = 1; j <= slices; ++j)
        {
            meshData.i[iIndex++] = (i - 1) * (slices + 1) + j;
            meshData.i[iIndex++] = (i - 1) * (slices + 1) + j % (slices + 1) + 1;
            meshData.i[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;

            meshData.i[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;
            meshData.i[iIndex++] = i * (slices + 1) + j;
            meshData.i[iIndex++] = (i - 1) * (slices + 1) + j;
        }
    }

    // 逐渐放入索引
    if (levels > 1)
    {
        for (UINT j = 1; j <= slices; ++j)
        {
            meshData.i[iIndex++] = (levels - 2) * (slices + 1) + j;
            meshData.i[iIndex++] = (levels - 2) * (slices + 1) + j % (slices + 1) + 1;
            meshData.i[iIndex++] = (levels - 1) * (slices + 1) + 1;
        }
    }

    return meshData;
}

MeshData GeometryGenerator::CreateTerrain() {
    MeshData data=CreateGrid(150.0f,150.0f,100,100);
    for(int i=0;i<data.vertex_count;++i){
        data.v[i].y=GetHeight(data.v[i].x,data.v[i].z);
        data.n[i]=GetNormal(data.v[i].x,data.v[i].z);
    }
    return data;
}