//
// Created by glodxy on 2019/12/22.
//

#ifndef GAME_WATER_H
#define GAME_WATER_H

#include "Plane.h"

namespace GRCore {
    class Water:public Plane{
    public:
        Water(int width,int depth);
        virtual ~Water()=default;

        void SetReflectCameraName(std::string name){
            mReflectCameraName=name;
        }
        std::string GetReflectCameraName()const{
            return mReflectCameraName;
        }

        void SetReflectTexture(std::string tex);
        void SetFlowDirection(XMFLOAT2 direction);
        void AddWave(Wave wave);

        void SetWave(Wave wave,int idx);
        Wave GetWave(int idx)const{
            return mTriggerWave[idx];
        }

        Wave* GetWave(){
            return mTriggerWave;
        }

        int GetWaveCount()const{
            return mWaveCount;
        }

        bool IsWaveChanged()const{
            return mWaveChanged;
        }

        XMFLOAT2 GetFlowDirection()const{
            return mDirection;
        }

        void ClearChangedFlag(){
            mWaveChanged=false;
        }
    protected:
        Wave mTriggerWave[10];
        std::string mReflectCameraName;
        int mWaveCount;
        bool mWaveChanged=false;
        XMFLOAT2 mDirection;
    };
}


#endif //GAME_WATER_H
