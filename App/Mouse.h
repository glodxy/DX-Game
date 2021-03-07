//
// Created by 田淙宇 on 2019/12/11.
//

#ifndef GAME_MOUSE_H
#define GAME_MOUSE_H

namespace GRCore {
    class Mouse {
    public:
        bool IsLeftPressed()const{
            return mLeftPressed;
        }
        bool IsRightPressed()const{
            return mRightPressed;
        }
        XMFLOAT2 GetPos()const{
            return XMFLOAT2(mX,mY);
        }

        void SetPos(float x,float y){
            mX=x;
            mY=y;
        }
        void SetLeftState(bool l){
            mLeftPressed=l;
        }
        void SetRightState(bool r){
            mRightPressed=r;
        }
    private:
        float mX, mY;
        bool mLeftPressed=false;
        bool mRightPressed=false;
    };
}


#endif //GAME_MOUSE_H
