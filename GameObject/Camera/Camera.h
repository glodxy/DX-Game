//
// Created by 田淙宇 on 2019/12/5.
//

#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include "../GameObject.h"

namespace GRCore {
    class Camera :public GameObject{
    public:
        Camera();
        void SetPosition(float x,float y,float z);
        void SetFOV(float fov);
        void RotateX(float angle);
        void RotateY(float angle);
        void Walk(float d);//前后
        void Strafe(float d);//左右
        virtual ~Camera()=default;

        void SetDirection(XMFLOAT3 up,XMFLOAT3 lookat);

        XMFLOAT4X4 GetViewMatrix()const;
        XMFLOAT4X4 GetProjMatrix()const;
        XMFLOAT4X4 GetOrthoMatrix()const;
        XMFLOAT4X4 GetViewProjMatrix()const;

        void OnKeyDown(char key)override;
        void OnMouseMove(uint32_t param,int x,int y);

        std::string GetRenderTarget()const;
        void SetRenderTarget(const std::string& rtv);

        XMFLOAT3 GetUpDirection()const{
            return mUp;
        }

        XMFLOAT3 GetRightDirection()const{
            return mRight;
        }

        XMFLOAT3 GetLookAtDirection()const{
            return mLookAt;
        }

        void SetCullingMask(int mask){
            mCullingMask=mask;
        }

        int GetCullingMask()const{
            return mCullingMask;
        }

        void SetProjType(bool ortho){
            mOrtho=ortho;
        }

        bool IsOrtho()const{
            return mOrtho;
        }
    protected:
        void UpdateValue();

        int mCullingMask;
        bool mOrtho=false;

        std::string mRenderTarget;
        float mStep;

        XMFLOAT3 mUp;
        XMFLOAT3 mRight;
        XMFLOAT3 mLookAt;


        float mAspect;
        float mFOV;
        float mNearZ;
        float mFarZ;

        float mLastMouseX;
        float mLastMouseY;
    };
}


#endif //GAME_CAMERA_H
