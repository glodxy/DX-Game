#include <iostream>
#include <d3dcompiler.h>
#define _DEBUG_OUT
#include "TestApp.h"


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE prevInstance,PSTR cmdLine,int showCmd){
#ifdef _DEBUG_OUT
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
    freopen("debug.log","w",stdout);
#endif
    freopen("error.log","w",stderr);

    try{
        TestApp app(hInstance);
        if(!app.Initialize(800,600)){
            return 0;
        }
        return app.Run();
    }catch(DxException &  e){
        std::cerr<<WStringToAnsi(e.ToString())<<std::endl;
        MessageBox(nullptr,WStringToAnsi(e.ToString().c_str()).c_str(),"Failed",MB_OK);
        return 0;
    }
}