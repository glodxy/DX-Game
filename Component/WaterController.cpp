//
// Created by glodxy on 2019/12/22.
//

#include "WaterController.h"
#include "../GameObject/Water.h"
#include "Mesh.h"
#include "../GameObject/Camera/Camera.h"
#include "../GameObject/Scene.h"
using namespace GRCore;

void WaterController::update() {
    Water* water=(Water*)mObject;

    if(water->IsWaveChanged()){
        water->SetExtraData("g_wave",ExtraData(water->GetWave(),10* sizeof(Wave)));
        water->SetExtraData("g_waveCount",ExtraData(water->GetWaveCount()));
        water->ClearChangedFlag();
    }

    XMFLOAT4X4 matTrans=water->GetComponent<Mesh>()->GetModelTexTransform();
    float tu=matTrans(3,0);
    float tv=matTrans(3,1);

    XMFLOAT2 trans=water->GetFlowDirection();
    std::cout<<"get trans:"<<trans.x<<","<<trans.y<<std::endl;
    tu+=trans.x*GR_TIMER->DeltaTime();
    tv+=trans.y*GR_TIMER->DeltaTime();

    if(tu>1.0f){
        tu-=1.0f;
    }
    if(tv>1.0f){
        tv-=1.0f;
    }
    matTrans(3,0)=tu;
    matTrans(3,1)=tv;
    water->GetComponent<Mesh>()->SetModelTexTransform(matTrans);

    Camera* rc=water->GetScene()->FindCameraByName(water->GetReflectCameraName());
    water->SetExtraData("ReflectViewProj",ExtraData(rc->GetViewProjMatrix()));

}