//
// Created by 田淙宇 on 2019/12/9.
//

#ifndef GAME_GEOMETRYGENERATOR_H
#define GAME_GEOMETRYGENERATOR_H

#include "D3DUtil.h"

namespace GRCore {
    class GeometryGenerator {
    public:
        static MeshData CreateGrid(float width,float depth,UINT32 m,UINT32 n);
        static MeshData CreateTerrain();
        static MeshData CreateSphere(float radius,UINT levels,UINT slices);
    private:
        static float GetHeight(float x,float z){
            return .1f*(z*sinf(0.1f*x)+x*cosf(0.1f*z));
        }
        static XMFLOAT3 GetNormal(float x,float z){
            XMFLOAT3 n(-0.1f*sinf(0.1f*x)+0.01f*x*sinf(0.1f*z),1.0f,-0.01f*z*cosf(0.1f*x)-0.1f*cosf(0.1f*z));
            XMVECTOR un=XMVector3Normalize(XMLoadFloat3(&n));
            XMStoreFloat3(&n,un);
            return n;
        }
    };
}


#endif //GAME_GEOMETRYGENERATOR_H
