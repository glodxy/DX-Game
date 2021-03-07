//
// Created by 田淙宇 on 2019/9/8.
//

#ifndef GRENDERER_D3DAPP_H
#define GRENDERER_D3DAPP_H

#include<iostream>
#include "../BaseApp.h"
#include "D3DUtil.h"
#include "../WindowsTimer.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Camera/Camera.h"
#include "../GameObject/Light/DirectionLight.h"
#include "../GameObject/Light/PointLight.h"
#include "../GameObject/Light/SpotLight.h"
#include "D3DRenderTarget.h"


namespace GRCore {
    class D3DApp:public GApp {

    protected:
        D3DApp(const D3DApp &) = delete;

        D3DApp &operator=(const D3DApp &) = delete;

    public:
        D3DApp(HINSTANCE hInstance);
        virtual ~D3DApp();

        static D3DApp *GetApp();

        HINSTANCE AppInstance() const;

        HWND MainWnd() const;

        int Run()override;

        bool Initialize(int width=800,int height=600,bool fullScreen=false)override ;

        virtual LRESULT MsgProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        std::string GetWindowTitle();
        void SetWindowTitle(const std::string& title);


    protected:

        virtual void OnInitialize(){}

        void DrawCanvas(const GCanvas& canvas)override;

        virtual void Draw(const GameTimer &gt);
        void Update(const GameTimer& gt)override;
        void FixedUpdate()override;

        virtual void OnUpdate(const GameTimer& gt){}
        virtual void OnFixdedUpdate(){}



        void OnResize()override;

        //鼠标处理
        void OnMouseDown(WPARAM wParam, int x, int y) {
            if((wParam&MK_LBUTTON)!=0){
                mouse->SetLeftState(true);
            }
            if((wParam&MK_RBUTTON)!=0){
                mouse->SetRightState(true);
            }
        }

        void OnMouseUp(WPARAM wParam, int x, int y) {
            if((wParam&MK_LBUTTON)==0){
                mouse->SetLeftState(false);
            }
            if((wParam&MK_RBUTTON)==0){
                mouse->SetRightState(false);
            }
        }

        void OnMouseMove(WPARAM wParam, int x, int y) {
            mouse->SetPos(x,y);
        }

        void OnKeyDown(WPARAM wParam);

        void OnKeyUp(WPARAM wParam);

    protected:
        bool InitWindow();

        bool InitDirectx();

        bool InitGameObject();

        void CreateSwapChain();
        void CreateRtvAndDsvDescriptorHeaps();

        void InitViewport();

        void CalculateFrameStats()override;

        void LogAdapters();

        void ReleaseAll();

    protected:
        Scene* mCurScene;

        Keyboard* keyboard;
        Mouse* mouse;

        static D3DApp *mApp;

        bool mFullScreen=true;

        UINT mNumerator,mDenominator;

        bool mVsyncEnable=true;//限定为屏幕刷新率
    protected:

        HINSTANCE mAppInstance = nullptr;
        HWND mMainWindow = nullptr;

        McComPtr<IDXGIFactory> mDxgiFactory;
        McComPtr<IDXGISwapChain> mSwapChain;
        McComPtr<ID3D11Device> mDevice;
        McComPtr<ID3D11DeviceContext> mDeviceContext;
        McComPtr<ID3D11Texture2D> mBackBuffer;//目标缓存

        UINT m4xMsaaQuality=0;


        D3D11_VIEWPORT mViewport;

    protected:
        //继承类修改值
        D3D_DRIVER_TYPE mDriverType = D3D_DRIVER_TYPE_HARDWARE;
        DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
        DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;


    };
}



#endif //GRENDERER_D3DAPP_H
