//
// Created by 田淙宇 on 2019/11/29.
//

#include <iostream>
#include "D3DShader.h"

GRCore::D3DShader::D3DShader(ID3D11Device *device, HWND hwnd, ID3D11DeviceContext *context,bool isEffect) {
    mDevice=device;
    mHwnd=hwnd;
    mDeviceContext=context;
    mIsEffect=isEffect;
    mInputLayout.resize(10);
}

GRCore::D3DShader::~D3DShader() {
}

void GRCore::D3DShader::Reset() {
    mVertexShader.Reset();
    mFragShader.Reset();
    for(auto&iter:mInputLayout){
        iter.Reset();
    }
    mFX.Reset();
    mTech.Reset();
}

void GRCore::D3DShader::Initialize(const std::string &vsFile, const std::string &fsFile,D3D11_INPUT_ELEMENT_DESC inputLayout[],int layoutSize) {
    //设置输入布局
    std::cout<<"string vs:"<<vsFile<<std::endl;
    if(!InitializeShader(&AnsiToWString(vsFile)[0],&AnsiToWString(fsFile)[0],inputLayout, layoutSize)){
        std::cerr<<"error init shader"<<std::endl;
    }
}

bool GRCore::D3DShader::InitializeShader(WCHAR * vs, WCHAR * fs,D3D11_INPUT_ELEMENT_DESC layouts[],int layoutSize) {
    if(mIsEffect){
        std::cerr<<"error init shader, this is defined to be a effect"<<std::endl;
        return false;
    }
    ID3DBlob* vsBuffer;
    ID3DBlob* fsBuffer;
    ID3DBlob* err;

    if(FAILED(D3DCompileFromFile(vs,NULL,NULL,"VSMain","vs_5_0",D3DCOMPILE_ENABLE_STRICTNESS,0,&vsBuffer,&err))){
        ::MessageBox(0,"Create Shader Failed",0,0);
        std::string temp((char*)err->GetBufferPointer(),err->GetBufferSize());
        std::cerr<<temp<<std::endl;
        return false;
    }
    ThrowIfFailed(mDevice->CreateVertexShader(vsBuffer->GetBufferPointer(),vsBuffer->GetBufferSize(),NULL,mVertexShader.GetAddressOf()));

    if(FAILED(D3DCompileFromFile(fs,NULL,NULL,"PSMain","ps_5_0",D3DCOMPILE_ENABLE_STRICTNESS,0,&fsBuffer,&err))){
        ::MessageBox(0,"Create Shader Failed",0,0);
        std::string temp((char*)err->GetBufferPointer(),err->GetBufferSize());
        std::cerr<<temp<<std::endl;
        return false;
    }
    ThrowIfFailed(mDevice->CreatePixelShader(fsBuffer->GetBufferPointer(),fsBuffer->GetBufferSize(),NULL,mFragShader.GetAddressOf()));


    McComPtr <ID3D11InputLayout> tempInputLayout;
    UINT numElems=layoutSize/ sizeof(layouts[0]);
    ThrowIfFailed(mDevice->CreateInputLayout(layouts,numElems,vsBuffer->GetBufferPointer(),vsBuffer->GetBufferSize(),tempInputLayout.GetAddressOf()));

    mInputLayout[0]=tempInputLayout;
    ReleaseCom(vsBuffer);
    ReleaseCom(fsBuffer);
    ReleaseCom(err);
    return true;

}

bool GRCore::D3DShader::InitializeEffect(const std::string&fx) {
    if(!mIsEffect){
        std::cerr<<"error init effect, this is a shader"<<std::endl;
        return false;
    }
    ID3DBlob* source;
    ID3DBlob* error;
    if(FAILED(D3DCompileFromFile(&AnsiToWString(fx)[0],NULL,D3D_COMPILE_STANDARD_FILE_INCLUDE,NULL,"fx_5_0",D3DCOMPILE_ENABLE_STRICTNESS,0,&source,&error))){
        ::MessageBox(0,"Create Shader Failed",0,0);
        std::string temp((char*)error->GetBufferPointer(),error->GetBufferSize());
        std::cerr<<temp<<std::endl;
        return false;
    }
    ThrowIfFailed(D3DX11CreateEffectFromMemory(source->GetBufferPointer(),source->GetBufferSize(),0,mDevice.Get(),mFX.GetAddressOf()));

    mTech=mFX->GetTechniqueByIndex(0);
    D3DX11_TECHNIQUE_DESC desc;
    mTech->GetDesc(&desc);
    mPassNum=desc.Passes;
    mTechName=desc.Name;
    std::cout<<"tech name:"<<desc.Name<<std::endl;

    ReleaseCom(source);
    ReleaseCom(error);
    return true;
}

void GRCore::D3DShader::CreateFXInputLayout(int pass, D3D11_INPUT_ELEMENT_DESC inputLayout[],
                                         int layoutSize) {
    if(!mIsEffect){
        std::cerr<<"cannot setup inputlayout, this is not a effect"<<std::endl;
    }

    D3DX11_PASS_DESC passDesc;
    mTech->GetPassByIndex(pass)->GetDesc(&passDesc);

    McComPtr <ID3D11InputLayout> tempInputLayout;

    UINT numElems=layoutSize/ sizeof(inputLayout[0]);

    for(int i=0;i<numElems;++i){
        mInputs.push_back(inputLayout[i].SemanticName);
    }

    ThrowIfFailed(mDevice->CreateInputLayout(inputLayout,numElems,passDesc.pIAInputSignature,passDesc.IAInputSignatureSize,tempInputLayout.GetAddressOf()));
    mInputLayout[pass]=tempInputLayout;

}



void GRCore::D3DShader::Use(int pass) {
    mDeviceContext->IASetInputLayout(mInputLayout[pass].Get());
    if(!mIsEffect) {
        mDeviceContext->VSSetShader(mVertexShader.Get(), NULL, 0);
        mDeviceContext->PSSetShader(mFragShader.Get(), NULL, 0);
    }else{
        ID3DX11EffectPass* Pass;
        McComPtr<ID3DX11EffectTechnique> temp;
        temp=mFX->GetTechniqueByName((mTechName+GR_CURRENT_MODE).c_str());
        if(temp->IsValid()){
            Pass=temp->GetPassByIndex(pass);
        }else {
            Pass = mTech->GetPassByIndex(pass);
        }
        Pass->Apply(0,mDeviceContext.Get());
        temp.Reset();
    }
}

bool GRCore::D3DShader::ContainsInput(std::string name) {
    for(auto iter:mInputs){
        if(iter==name)
            return true;
    }
    return false;
}


void GRCore::D3DShader::SetMatrix4(const std::string &name, XMFLOAT4X4 matrix) {
    ID3DX11EffectMatrixVariable* matrixVariable=mFX->GetVariableByName(name.c_str())->AsMatrix();
    matrixVariable->SetMatrix(reinterpret_cast<float*>(&matrix));
}

void GRCore::D3DShader::SetMatrix4Array(const std::string &name, XMFLOAT4X4 *array,int count) {
    ID3DX11EffectMatrixVariable* matrixVariable=mFX->GetVariableByName(name.c_str())->AsMatrix();
    matrixVariable->SetMatrixArray(reinterpret_cast<float*>(array),0,count);
}

void GRCore::D3DShader::SetVector(const std::string &name, XMFLOAT4 vec) {
    ID3DX11EffectVectorVariable* vectorVariable=mFX->GetVariableByName(name.c_str())->AsVector();
    vectorVariable->SetFloatVector(reinterpret_cast<float*>(&vec));
}

void GRCore::D3DShader::SetVector(const std::string &name, XMFLOAT3 vec) {
    ID3DX11EffectVectorVariable* vectorVariable=mFX->GetVariableByName(name.c_str())->AsVector();
    vectorVariable->SetFloatVector(reinterpret_cast<float*>(&vec));
}

void GRCore::D3DShader::SetVector(const std::string &name, XMFLOAT2 vec) {
    ID3DX11EffectVectorVariable* vectorVariable=mFX->GetVariableByName(name.c_str())->AsVector();
    vectorVariable->SetFloatVector(reinterpret_cast<float*>(&vec));
}

void GRCore::D3DShader::SetTexture(const std::string &name,ID3D11ShaderResourceView* v) {
    ID3DX11EffectShaderResourceVariable* shaderResourceVariable=mFX->GetVariableByName(name.c_str())->AsShaderResource();
    shaderResourceVariable->SetResource(v);
}

void GRCore::D3DShader::SetTextureArray(const std::string &name, ID3D11ShaderResourceView **pv, int count) {
    ID3DX11EffectShaderResourceVariable* shaderResourceVariable=mFX->GetVariableByName(name.c_str())->AsShaderResource();
    shaderResourceVariable->SetResourceArray(pv,0,count);
}

void GRCore::D3DShader::SetRenderTarget(const std::string &name, ID3D11RenderTargetView *rtv) {
    ID3DX11EffectRenderTargetViewVariable* variable=mFX->GetVariableByName(name.c_str())->AsRenderTargetView();
    variable->SetRenderTarget(rtv);
}

void GRCore::D3DShader::SetRenderTargets(const std::string &name, ID3D11RenderTargetView **rtv, int count) {
    ID3DX11EffectRenderTargetViewVariable* variable=mFX->GetVariableByName(name.c_str())->AsRenderTargetView();
    variable->SetRenderTargetArray(rtv,0,count);
}

void GRCore::D3DShader::SetInt(const std::string &name, int value) {
    ID3DX11EffectScalarVariable* vectorVariable=mFX->GetVariableByName(name.c_str())->AsScalar();
    vectorVariable->SetInt(value);
}

void GRCore::D3DShader::SetFloat(const std::string &name, float value) {
    ID3DX11EffectScalarVariable* floatVariable=mFX->GetVariableByName(name.c_str())->AsScalar();
    floatVariable->SetFloat(value);
}
