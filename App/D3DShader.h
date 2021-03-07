//
// Created by 田淙宇 on 2019/11/29.
//

#ifndef GAME_D3DSHADER_H
#define GAME_D3DSHADER_H

#include "D3DUtil.h"

namespace GRCore {
    class D3DApp;
    class D3DShader {
    public:
        D3DShader(ID3D11Device* device,HWND,ID3D11DeviceContext*, bool isEffect=true);
        D3DShader(const D3DShader& s)=delete;
        ~D3DShader();

        void Reset();
    public:
        void Initialize(const std::string& vsFile,const std::string& fsFile,D3D11_INPUT_ELEMENT_DESC inputLayout[],int layoutSize);
        void Use(int pass=0);

        bool ContainsInput(std::string name);

        //初始化effect
        bool InitializeEffect(const std::string& fileName);
        //设置对应的输入布局
        void CreateFXInputLayout(int pass,D3D11_INPUT_ELEMENT_DESC inputLayout[],int layoutSize);

        int GetPassNum()const{
            return mPassNum;
        }

        bool IsEffect()const{
            return mIsEffect;
        }

        void SetExtraData(ExtraData data);

        void SetMatrix4(const std::string& name,XMFLOAT4X4 matrix);
        void SetMatrix4Array(const std::string& name,XMFLOAT4X4* array,int count);
        void SetVector(const std::string&name,XMFLOAT4 vec);
        void SetVector(const std::string&name,XMFLOAT3 vec);
        void SetVector(const std::string&name,XMFLOAT2 vec);
        void SetInt(const std::string&name,int value);
        void SetFloat(const std::string&name,float value);
        void SetTexture(const std::string&name,ID3D11ShaderResourceView*v);
        void SetTextureArray(const std::string&name,ID3D11ShaderResourceView**pv,int count);
        void SetRenderTarget(const std::string&name,ID3D11RenderTargetView*rtv);
        void SetRenderTargets(const std::string&name,ID3D11RenderTargetView**rtv,int counnt);
        template<class T>
        void SetStruct(const std::string&name,T* a,int count=1){
            ID3DX11EffectVariable* variable=mFX->GetVariableByName(name.c_str());
            variable->SetRawValue(a,0, sizeof(T)*count);
        }

        void SetStruct(const std::string&name,void*a,int size){
            ID3DX11EffectVariable* variable=mFX->GetVariableByName(name.c_str());
            variable->SetRawValue(a,0, size);
        }
    private:
        bool InitializeShader(WCHAR*, WCHAR*,D3D11_INPUT_ELEMENT_DESC layouts[],int layoutNum);

    private:
        bool mIsEffect=true;

        std::vector<std::string> mInputs;
        McComPtr<ID3D11DeviceContext> mDeviceContext;
        McComPtr<ID3D11Device> mDevice;
        HWND mHwnd;

        McComPtr<ID3D11VertexShader> mVertexShader;
        McComPtr<ID3D11PixelShader> mFragShader;
        std::vector<McComPtr<ID3D11InputLayout>> mInputLayout;

        McComPtr <ID3DX11Effect> mFX;
        McComPtr <ID3DX11EffectTechnique> mTech;
        std::string mTechName;
        int mPassNum;
    };
}


#endif //GAME_D3DSHADER_H
