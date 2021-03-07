//
// Created by 田淙宇 on 2019/9/8.
//

#include "D3DApp.h"
#include "../GameObject/Scene.h"
#include "D3DSourceManager.h"
#include <windowsx.h>

LRESULT CALLBACK
MainWndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
    return GRCore::D3DApp::GetApp()->MsgProcess(hWnd,msg,wParam,lParam);
}

GRCore::D3DApp* GRCore::D3DApp::mApp= nullptr;
GRCore::D3DApp* GRCore::D3DApp::GetApp() {
    return mApp;
}

GRCore::D3DApp::D3DApp(HINSTANCE hInstance):mAppInstance(hInstance) {
    assert(mApp== nullptr);
    mApp=this;
}

GRCore::D3DApp::~D3DApp() {
    mCurScene->Destroy();
}

HINSTANCE GRCore::D3DApp::AppInstance() const {
    return mAppInstance;
}

HWND GRCore::D3DApp::MainWnd() const {
    return mMainWindow;
}

void GRCore::D3DApp::ReleaseAll() {
    mBackBuffer.Reset();
    mDevice.Reset();
    mDeviceContext.Reset();
    D3DSourceManager::ResetAll();
}

bool GRCore::D3DApp::Initialize(int width,int height,bool fullScreen) {
    mFullScreen=fullScreen;
    if(!fullScreen) {
        mWidth = width;
        mHeight = height;
    }
    if(!InitWindow())
        return false;
    if(!InitDirectx())
        return false;

    GR_DEVICE=mDevice.Get();
    GR_CONTEXT=mDeviceContext.Get();
    GR_WINDOW=mMainWindow;
    GR_TIMER=&mTimer;
    GR_WIDTH=mWidth;
    GR_HEIGHT=mHeight;
    GR_MSAA_QUALITY=m4xMsaaQuality;

    mouse=new Mouse();
    keyboard=new Keyboard();
    GR_MOUSE=mouse;
    GR_KEYBOARD=keyboard;

    OnInitialize();

    InitGameObject();
    D3DSourceManager::GetSourceManager()->LoadRenderTarget(DEFAULT_RENDERTARGET_NAME,mBackBuffer);
    std::cout<<"start load main rtv"<<std::endl;
    return true;
}

LRESULT GRCore::D3DApp::MsgProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg){
        //激活或非激活时
        case WM_ACTIVATE:{
            //激活时启动，否则暂停
            if(LOWORD(wParam)==WA_INACTIVE){
                mAppPaused=true;
                mTimer.Stop();
            }
            else{
                mAppPaused=false;
                mTimer.Start();
            }
            return 0;
        }

        case WM_SIZE:{
            mWidth=LOWORD(lParam);
            mHeight=HIWORD(lParam);
            if(mDevice){
                //最小化时
                if(wParam==SIZE_MINIMIZED){
                    mAppPaused=true;
                    mMinimized=true;
                    mMaxiMized=false;
                }
                //最大化时
                else if(wParam==SIZE_MAXIMIZED){
                    mAppPaused=false;
                    mMinimized=false;
                    mMaxiMized=true;
                    OnResize();
                }
                //重设大小
                else if(wParam==SIZE_RESTORED){
                    //从最小化改变大小
                    if(mMinimized){
                        mAppPaused=false;
                        mMinimized=false;
                        OnResize();
                    }
                    //从最大化变化
                    else if(mMaxiMized){
                        mAppPaused=false;
                        mMaxiMized=false;
                        OnResize();
                    }
                    else if(mResizing){

                    }
                    else{
                        OnResize();
                    }
                }
            }
            return 0;
        }
        //拖动改变窗口大小时
        case WM_ENTERSIZEMOVE:{
            mAppPaused=true;
            mResizing=true;
            mTimer.Stop();
            return 0;
        }
        //停止改变窗口大小时
        case WM_EXITSIZEMOVE:{
            mAppPaused=false;
            mResizing=false;
            mTimer.Start();
            OnResize();
            return 0;
        }
        //窗口关闭时
        case WM_DESTROY:{
            PostQuitMessage(0);
            return 0;
        }
        //用户点击窗口按键时
        case WM_MENUCHAR:{
            return MAKELRESULT(0,MNC_CLOSE);
        }
        //鼠标按键
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:{
            OnMouseDown(wParam,GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
            return 0;
        }
            //鼠标移动
        case WM_MOUSEMOVE: {
            OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        }

        //鼠标松开
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        }

        case WM_KEYDOWN:{
            OnKeyDown(wParam);
            return 0;
        }
        //按键
        case WM_KEYUP:{
            OnKeyUp(wParam);
            return 0;
        }
    }
    return DefWindowProc(hWnd,msg,wParam,lParam);
}

bool GRCore::D3DApp::InitWindow() {
    WNDCLASS wc;
    wc.style=CS_HREDRAW|CS_VREDRAW;
    wc.lpfnWndProc=MainWndProc;
    wc.cbClsExtra=0;
    wc.cbWndExtra=0;
    wc.hInstance=mAppInstance;
    wc.hIcon=LoadIcon(0,IDI_HAND);
    wc.hCursor=LoadCursor(0,IDC_CROSS);
    wc.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH );
    wc.lpszMenuName=0;
    wc.lpszClassName="MainWnd";

    if(!RegisterClass(&wc)){
        MessageBox(0,"Regist class failed",0,0);
        return false;
    }

    RECT rect={0,0,mWidth,mHeight};
    AdjustWindowRect(&rect,WS_OVERLAPPEDWINDOW,false);
    int width=rect.right-rect.left;
    int height=rect.bottom-rect.top;

    mMainWindow=CreateWindow("MainWnd",mMainWindowCaption.c_str(),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,width,height,0,0,mAppInstance,0);

    if(!mMainWindow){
        MessageBox(0,"Create Window Failed",0,0);
        return false;
    }

    ShowWindow(mMainWindow,SW_SHOW);
    UpdateWindow(mMainWindow);

    return true;
}

//绘制canvas
void GRCore::D3DApp::DrawCanvas(const GRCore::GCanvas &canvas) {
}




bool GRCore::D3DApp::InitDirectx() {
    int videoCardMemory;
    std::string videoCardName;

    IDXGIAdapter* adapter;//适配器
    IDXGIOutput* adapterOutput;//适配器输出
    UINT numModes;
    DXGI_MODE_DESC* displayModeList;
    DXGI_ADAPTER_DESC adapterDesc;
    UINT numerator,denominator;
#if defined(DEBUG)||defined(_DEBUG)
{

}
#endif
    ThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(&mDxgiFactory)));

    ThrowIfFailed(mDxgiFactory->EnumAdapters(0,&adapter));

    ThrowIfFailed(adapter->EnumOutputs(0,&adapterOutput));
    ThrowIfFailed(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL));

    displayModeList=new DXGI_MODE_DESC[numModes];
    if(!displayModeList)
        return false;

    ThrowIfFailed(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList));

    //浏览所有的显示模式,找到适合屏幕宽度和高度的
    //当一个模式匹配,存储监视器刷新速度的分子分母??
    for (int i = 0; i<numModes; i++)
    {
        if (displayModeList[i].Width == (unsigned int)mWidth)
        {
            if (displayModeList[i].Height == (unsigned int)mHeight)
            {
                numerator = displayModeList[i].RefreshRate.Numerator;
                denominator = displayModeList[i].RefreshRate.Denominator;
            }
        }
    }

    //获取适配器(显卡)形容
    ThrowIfFailed(adapter->GetDesc(&adapterDesc));

    videoCardMemory=(int)(adapterDesc.DedicatedVideoMemory/1024/1024);
    videoCardName=WStringToAnsi(adapterDesc.Description);
    delete[] displayModeList;
    displayModeList = NULL;
    mNumerator=numerator;
    mDenominator=denominator;

    ReleaseCom(adapter);
    ReleaseCom(adapterOutput);


#ifdef _DEBUG
    LogAdapters();
#endif
    CreateSwapChain();
    std::cout<<"init swapchain over"<<std::endl;
    CreateRtvAndDsvDescriptorHeaps();
    std::cout<<"init rtv and dsv over"<<std::endl;
    InitViewport();
    std::cout<<"init vp over"<<std::endl;


//#if (_WIN32_WINNT >= 0x0A00 /*_WIN32_WINNT_WIN10*/)
//    Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
//    if (FAILED(initialize))
//        // error
//#else
//        HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
//    if (FAILED(hr))
//        // error
//#endif

    return true;

}

void GRCore::D3DApp::CreateSwapChain() {
    D3D_FEATURE_LEVEL featureLevel;
    featureLevel = D3D_FEATURE_LEVEL_11_0;
    ThrowIfFailed(D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,0,&featureLevel,1,D3D11_SDK_VERSION,mDevice.GetAddressOf(),NULL,mDeviceContext.GetAddressOf()));

    mSwapChain.Reset();
    std::cout<<"start init multisample"<<std::endl;
    ThrowIfFailed(mDevice->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
    std::cout<<"msaa quality:"<<m4xMsaaQuality<<std::endl;
    if(m4xMsaaQuality<=0){
        std::cerr<<"error create multisample"<<std::endl;
    }
    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = mWidth;
    sd.BufferDesc.Height = mHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    if (mVsyncEnable) //限不限帧
    {
        sd.BufferDesc.RefreshRate.Numerator =mNumerator;
        sd.BufferDesc.RefreshRate.Denominator = mDenominator;
    }
    else
    {
        sd.BufferDesc.RefreshRate.Numerator = 0;
        sd.BufferDesc.RefreshRate.Denominator = 1;
    }

    sd.SampleDesc.Count = 4;
    sd.SampleDesc.Quality = m4xMsaaQuality-1;

    //是否进行全屏
    if (mFullScreen)
    {
        sd.Windowed = false;
    }
    else
    {
        sd.Windowed = true;
    }
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;  //背后缓存数量
    sd.OutputWindow = mMainWindow; //交换链所属的窗口
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    std::cout<<"start create swapchain"<<std::endl;
    ThrowIfFailed(mDxgiFactory->CreateSwapChain(mDevice.Get(),&sd,mSwapChain.GetAddressOf()));
    mDxgiFactory.Reset();
}

void GRCore::D3DApp::CreateRtvAndDsvDescriptorHeaps() {
    mSwapChain->GetBuffer(0,IID_PPV_ARGS(mBackBuffer.GetAddressOf()));

}

void GRCore::D3DApp::InitViewport() {
    mViewport.Width= static_cast<float>(mWidth);
    mViewport.Height= static_cast<float>(mHeight);
    mViewport.MinDepth=0.0f;
    mViewport.MaxDepth=1.0f;
    mViewport.TopLeftX=.0f;
    mViewport.TopLeftY=.0f;
    mDeviceContext->RSSetViewports(1,&mViewport);
}

void GRCore::D3DApp::LogAdapters() {

}

bool GRCore::D3DApp::InitGameObject() {
    if(GR_DEVICE== nullptr||GR_CONTEXT== nullptr){
        std::cerr<<"not ready"<<std::endl;
    }
    std::cout<<"env variable set finished"<<std::endl;

    mCurScene->Awake();
    return true;
}

//计算帧数
void GRCore::D3DApp::CalculateFrameStats() {
    static int frameCount=0;
    static float timeElapsed=0.0f;

    frameCount++;

    if((mTimer.TotalTime()-timeElapsed)>=1.0f){
        float fps=(float)frameCount;
        float mspf=1000.0f/fps;

        std::string fpsStr=std::to_string(fps);
        std::string mspfStr=std::to_string(mspf);

        std::string title=mMainWindowCaption+"    fps:"+fpsStr+"     mspf:"+mspfStr;

        SetWindowText(mMainWindow,title.c_str());

        frameCount=0;
        timeElapsed+=1.0f;
    }
}

int GRCore::D3DApp::Run() {
    MSG msg={0};

    mTimer.Reset();

    while(msg.message!=WM_QUIT){
        if(PeekMessage(&msg,0,0,0,PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else{
            mTimer.Tick();

            if(!mAppPaused){
                CalculateFrameStats();
                Update(mTimer);
                Draw(mTimer);
                float curTime=mTimer.DeltaTime();
                while(curTime+mLastTime>mFixedTime){
                    FixedUpdate();
                    curTime-=mFixedTime;
                }
                mLastTime+=curTime;

            }
            else{
                Sleep(100);
            }
        }
    }
    ReleaseAll();
    return (int)msg.wParam;
}

void GRCore::D3DApp::OnKeyDown(WPARAM wParam){
    keyboard->Press(wParam);
#ifdef _DEBUG_OUT
    std::cout<<"key down:"<<(char)wParam<<std::endl;
#endif
}

void GRCore::D3DApp::OnKeyUp(WPARAM wParam) {
    keyboard->Release(wParam);
#ifdef _DEBUG_OUT
    std::cout<<"key up:"<<wParam<<std::endl;
#endif
}

void GRCore::D3DApp::OnResize() {
}

void GRCore::D3DApp::SetWindowTitle(const std::string &title) {}

std::string GRCore::D3DApp::GetWindowTitle() {
    return "";
}

void GRCore::D3DApp::Draw(const GRCore::GameTimer &gt) {

}

void GRCore::D3DApp::Update(const GRCore::GameTimer &gt){

    OnUpdate(gt);
    mCurScene->Update();
    mSwapChain->Present(0,0);
}

void GRCore::D3DApp::FixedUpdate() {
    OnFixdedUpdate();
    mCurScene->FixedUpdate();
}
