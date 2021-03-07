//
// Created by glodxy on 2019/12/22.
//

#include "Water.h"
#include "../Component/Transform.h"
#include "../Component/Mesh.h"
#include "../Component/WaterController.h"

using namespace GRCore;

Water::Water(int width, int depth):Plane(){
    mWaveCount=0;
    SetLayer(2);
    SetExtraData("g_reflection",ExtraData("reflection_texture"));
    SetExtraData("g_wave",ExtraData(mTriggerWave,10));
    SetExtraData("g_waveCount",ExtraData(mWaveCount));

    float scaleX=(float)width/100;
    float scaleZ=(float)depth/100;
    GetComponent<Transform>()->SetScale(scaleX,1.0f,scaleZ);

    XMFLOAT4X4 t;
    Mesh*pm=GetComponent<Mesh>();
    XMStoreFloat4x4(&t,XMMatrixScaling(2.0f,2.0f,1.0f));
    pm->SetTexture({"water_texture",t});
    pm->SetMaterialName("water");
    pm->SetShaderName("WaterShader");

    AddComponent(new WaterController());
}

void Water::SetReflectTexture(std::string tex) {
    SetExtraData("g_reflection",ExtraData(tex));
}

void Water::SetFlowDirection(XMFLOAT2 direction) {
    mDirection=direction;
}

void Water::AddWave(GRCore::Wave wave) {
    mTriggerWave[mWaveCount]=wave;
    ++mWaveCount;
    mWaveChanged=true;
    std::cout<<"add wave:"<<wave.Amplitude<<","<<wave.Frequency<<","<<wave.Length<<std::endl;
    std::cout<<"wave count:"<<mWaveCount<<std::endl;
}

void Water::SetWave(GRCore::Wave wave, int idx) {
    if(idx>=10){
        std::cerr<<"set wave error:"<<GetName()<<", max size:"<<10<<", target:"<<idx<<std::endl;
    }
    mTriggerWave[idx]=wave;
    mWaveChanged=true;
}