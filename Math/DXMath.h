//
// Created by 田淙宇 on 2019/11/25.
//

#ifndef GAME_DXMATH_H
#define GAME_DXMATH_H

#include <iostream>
#include <DirectXPackedVector.h>
#include <DirectXMath.h>
using namespace DirectX;
static std::ostream& operator<<(std::ostream&os,XMVECTOR u){
    os<<"("<<XMVectorGetX(u)<<","<<XMVectorGetY(u)<<","<<XMVectorGetZ(u)<<","<<XMVectorGetW(u)<<")"<<std::endl;
    return os;
}

static std::ostream& operator<<(std::ostream& os,XMFLOAT4X4 m){
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            os<<" "<<m(i,j)<<" ";
        }
        os<<std::endl;
    }
    os<<";"<<std::endl;
    return os;
}

#endif //GAME_DXMATH_H
