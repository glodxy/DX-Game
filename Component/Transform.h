//
// Created by 田淙宇 on 2019/12/5.
//

#ifndef GAME_TRANSFORM_H
#define GAME_TRANSFORM_H

#include "BaseComponent.h"

namespace GRCore {
    class Transform :public BaseComponent{
    public:
        Transform();
        Transform(float* pos,float* rotation,float* scale);


        void SetPosition(float x,float y,float z);
        void SetRotation(float x,float y,float z);
        void SetScale(float x,float y,float z);

        //获取位置
        XMFLOAT3 GetPosition()const{
            return XMFLOAT3(mX,mY,mZ);
        }
        //获取旋转
        XMFLOAT3 GetRotation()const{
            return XMFLOAT3(mRX,mRY,mRZ);
        }
        //获取缩放
        XMFLOAT3 GetScale()const{
            return XMFLOAT3(mSX,mSY,mSZ);
        }

        //获取模型矩阵
        XMFLOAT4X4 GetMatrix()const;

        virtual std::string getName()const{
            return "Transform";
        }

    protected:
        float mX,mY,mZ;
        float mRX,mRY,mRZ;
        float mSX,mSY,mSZ;
    };
}


#endif //GAME_TRANSFORM_H
