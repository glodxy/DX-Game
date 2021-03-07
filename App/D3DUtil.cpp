//
// Created by 田淙宇 on 2019/9/9.
//

#include "D3DUtil.h"
#include <comdef.h>
#include <fstream>
using namespace GRCore;
using Microsoft::WRL::ComPtr;

GameTimer* GRCore::GR_TIMER=nullptr;

ID3D11Device *GRCore::GR_DEVICE= nullptr;
ID3D11DeviceContext *GRCore::GR_CONTEXT= nullptr;
HWND GRCore::GR_WINDOW= nullptr;

int GRCore::GR_WIDTH=0;
int GRCore::GR_HEIGHT=0;
int GRCore::GR_MSAA_QUALITY=1;

std::string GRCore::GR_CURRENT_MODE="";

D3D11_VIEWPORT GRCore::GR_CURRENT_VIEWPORT={.0f,.0f,800.0f,600.0f,0.0f,1.0f};

int GRCore::GR_CULLING_MASK=0;
XMFLOAT3 GRCore::GR_CAMERA_POS=XMFLOAT3();
XMFLOAT4X4 GRCore::GR_VIEW_MATRIX= XMFLOAT4X4();
XMFLOAT4X4 GRCore::GR_PROJ_MATRIX=XMFLOAT4X4();
XMFLOAT4X4 GRCore::GR_MODEL_MATRIX=XMFLOAT4X4();
XMFLOAT4X4 GRCore::GR_NORMAL_WORLD_MATRIX=XMFLOAT4X4();
XMFLOAT4X4 GRCore::GR_MODEL_TEX_TRANSFORM_MATRIX=XMFLOAT4X4();
XMFLOAT4X4 GRCore::GR_LIGHT_VIEWPROJ_MATRIX[4]={};
XMFLOAT4X4* GRCore::GR_BONES_MATRIX=nullptr;

std::string GRCore::GR_SKYBOX_TEXTURE="";

Mouse* GRCore::GR_MOUSE=nullptr;
Keyboard* GRCore::GR_KEYBOARD=nullptr;

XMFLOAT4 GRCore::GR_AMBIENT_LIGHT=XMFLOAT4(.5f,.5f,.5f,1.0f);
LIGHT GRCore::GR_LIGHTS[30]={};

int GRCore::GR_DIRLIGHT_SIZE=0;
int GRCore::GR_POINTLIGHT_SIZE=0;
int GRCore::GR_SPOTLIGHT_SIZE=0;

DxException::DxException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber) :
        ErrorCode(hr),
        FunctionName(functionName),
        Filename(filename),
        LineNumber(lineNumber)
{
}


std::wstring DxException::ToString()const
{
    // Get the string description of the error code.
    _com_error err(ErrorCode);
    std::wstring msg = AnsiToWString(err.ErrorMessage());

    return FunctionName + L" failed in " + Filename + L"; line " + std::to_wstring(LineNumber) + L"; error: " + msg;
}

void GRCore::ExtraData::ClearData() {
    switch(dataType){
        case GR_DATATYPE_FLOAT3:{
            XMFLOAT3* pd= reinterpret_cast<XMFLOAT3*>(data);
            delete pd;
            break;
        }
        case GR_DATATYPE_FLOAT2:{
            XMFLOAT2* pd= reinterpret_cast<XMFLOAT2*>(data);
            delete pd;
            break;
        }
        case GR_DATATYPE_INT:{
            int* pd= reinterpret_cast<int*>(data);
            delete pd;
            break;
        }
        case GR_DATATYPE_MATRIX:{
            XMFLOAT4X4* pd= reinterpret_cast<XMFLOAT4X4*>(data);
            delete pd;
            break;
        }
        case GR_DATATYPE_RENDERTARGET:
        case GR_DATATYPE_TEXTURE:{
            char* str= reinterpret_cast<char*>(data);
            delete[] str;
            break;
        }
        case GR_DATATYPE_STRUCT:{
            delete[] (char*)data;
            break;
        }
    }
    data=nullptr;
}

GRCore::ExtraData::ExtraData(){
    dataType=GR_DATATYPE_NONE;
    data=nullptr;
    dataSize=0;
}

GRCore::ExtraData::ExtraData(void *p, int size) {
    dataType=GR_DATATYPE_STRUCT;
    this->data=new char[size];
    dataSize=size;
    memcpy(this->data,(char*)p,dataSize);
}

GRCore::ExtraData::ExtraData(XMFLOAT4X4 data) {
    dataType=GR_DATATYPE_MATRIX;
    XMFLOAT4X4* pd=new XMFLOAT4X4(data);
    this->data=pd;
    dataSize= sizeof(XMFLOAT4X4);
}

GRCore::ExtraData::ExtraData(XMFLOAT3 data) {
    dataType=GR_DATATYPE_FLOAT3;
    XMFLOAT3* pd=(new XMFLOAT3(data));
    this->data=pd;
    dataSize= sizeof(XMFLOAT3);
}

GRCore::ExtraData::ExtraData(XMFLOAT2 data) {
    dataType=GR_DATATYPE_FLOAT2;
    XMFLOAT2* pd=new XMFLOAT2(data);
    this->data=pd;
    dataSize= sizeof(XMFLOAT2);
}

GRCore::ExtraData::ExtraData(int data) {
    dataType=GR_DATATYPE_INT;
    int* pd=new int;
    *pd=data;
    this->data=pd;
    dataSize= sizeof(int);
}

GRCore::ExtraData::ExtraData(const std::string &data,bool rtv) {
    if(rtv){
        dataType=GR_DATATYPE_RENDERTARGET;
    }else {
        dataType = GR_DATATYPE_TEXTURE;
    }
    char* pd=new char[data.size()+1];
    memcpy(pd,data.c_str(),data.size()+1);
    this->data=pd;
    dataSize=data.size()+1;
}

GRCore::ExtraData::ExtraData(ExtraDataType type){
    dataType=type;
    data=nullptr;
    dataSize=0;
}

GRCore::ExtraData::ExtraData(const GRCore::ExtraData &d) {
    dataType=d.dataType;
    char*pd=new char[d.dataSize];
    dataSize=d.dataSize;
    memcpy(pd,(char*)d.data,d.dataSize);
    this->data=pd;
    //CopyData(d.dataType,d.data);
}

ExtraData& GRCore::ExtraData::operator=(const GRCore::ExtraData &r) {
    if(this->data!=nullptr){
        ClearData();
    }
    dataType=r.dataType;
    char*pd=new char[r.dataSize];
    dataSize=r.dataSize;
    memcpy(pd,(char*)r.data,r.dataSize);
    this->data=pd;
    return *this;
}

GRCore::ExtraData::~ExtraData() {
    ClearData();
}