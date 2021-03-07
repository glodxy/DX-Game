//
// Created by 田淙宇 on 2019/12/5.
//

#include "Camera.h"
#include "../../Component/Transform.h"
#include "../../Component/CameraController.h"

using namespace GRCore;

Camera::Camera():GameObject(),mUp(.0f,1.0,.0f),mRight(1.0f,.0f,.0f),mLookAt(.0f,.0f,1.0f),mStep(.1f){
    mAspect=800.0f/600.0f;
    mFOV=0.25f*XM_PI;
    mNearZ=0.1f;
    mFarZ=1000.0f;
    mRenderTarget=DEFAULT_RENDERTARGET_NAME;
    UpdateValue();
}

void Camera::UpdateValue() {
    Transform *transform=GetComponent<Transform>();
    XMVECTOR right=XMLoadFloat3(&mRight);
    XMVECTOR look=XMLoadFloat3(&mLookAt);
    std::cout<<"origin right"<<right<<std::endl;
    std::cout<<"origin look"<<look<<std::endl;
    look=XMVector3Normalize(look);
    std::cout<<"norma look"<<look<<std::endl;
    XMVECTOR up=XMVector3Normalize(XMVector3Cross(look,right));
    right=XMVector3Cross(up,look);

    std::cout<<"update camera look:"<<look<<std::endl;
    std::cout<<"up:"<<up<<std::endl;
    std::cout<<"right:"<<right<<std::endl;

    XMStoreFloat3(&mRight,right);
    XMStoreFloat3(&mUp,up);
    XMStoreFloat3(&mLookAt,look);
}

void Camera::SetPosition(float x, float y, float z) {
    Transform *transform=GetComponent<Transform>();
    transform->SetPosition(x,y,z);
    UpdateValue();
}

void Camera::SetFOV(float fov) {
    mFOV=fov;
}

void Camera::Strafe(float d) {
    Transform *transform=GetComponent<Transform>();
    XMFLOAT3 pos=transform->GetPosition();

    XMVECTOR s=XMVectorReplicate(d);
    XMVECTOR r=XMLoadFloat3(&mRight);
    XMVECTOR p=XMLoadFloat3(&pos);
    XMStoreFloat3(&pos,XMVectorMultiplyAdd(s,r,p));

    transform->SetPosition(pos.x,pos.y,pos.z);
    UpdateValue();
}

void Camera::Walk(float d) {
    Transform *transform=GetComponent<Transform>();
    XMFLOAT3 pos=transform->GetPosition();

    XMVECTOR s=XMVectorReplicate(d);
    XMVECTOR l=XMLoadFloat3(&mLookAt);
    XMVECTOR p=XMLoadFloat3(&pos);
    XMStoreFloat3(&pos,XMVectorMultiplyAdd(s,l,p));

    transform->SetPosition(pos.x,pos.y,pos.z);
    UpdateValue();
}

void Camera::RotateX(float angle) {
    XMMATRIX R=XMMatrixRotationAxis(XMLoadFloat3(&mRight),angle);
    XMStoreFloat3(&mUp,XMVector3TransformNormal(XMLoadFloat3(&mUp),R));
    XMStoreFloat3(&mLookAt,XMVector3TransformNormal(XMLoadFloat3(&mLookAt),R));
    UpdateValue();
}

void Camera::RotateY(float angle) {
    XMMATRIX R=XMMatrixRotationY(angle);
    XMStoreFloat3(&mRight,XMVector3TransformNormal(XMLoadFloat3(&mRight),R));
    XMStoreFloat3(&mUp,XMVector3TransformNormal(XMLoadFloat3(&mUp),R));
    XMStoreFloat3(&mLookAt,XMVector3TransformNormal(XMLoadFloat3(&mLookAt),R));
    UpdateValue();
}

void Camera::OnMouseMove(uint32_t param, int x, int y) {
    if((param&MK_LBUTTON) !=0){
        float dx=XMConvertToRadians(0.25f* static_cast<float>(x-mLastMouseX));
        float dy=XMConvertToRadians(0.25f* static_cast<float>(y-mLastMouseY));

        RotateX(dy);
        RotateY(dx);
    }
    mLastMouseX=x;
    mLastMouseY=y;
}

void Camera::OnKeyDown(char key) {
    Transform *transform=GetComponent<Transform>();
    XMFLOAT3 pos=transform->GetPosition();
    switch(key){
        case 'A':{
            Strafe(-mStep);
            break;
        }
        case 'D':{
            Strafe(mStep);
            break;
        }
        case 'W':{
            Walk(mStep);
            break;
        }
        case 'S':{
            Walk(-mStep);
            break;
        }
    }
}

XMFLOAT4X4 Camera::GetViewMatrix() const {
    const Transform *transform=GetComponent<Transform>();
    XMVECTOR pos=XMLoadFloat3(&(transform->GetPosition()));
    XMVECTOR look=XMLoadFloat3(&mLookAt);
    XMVECTOR up=XMLoadFloat3(&mUp);

    XMFLOAT4X4 view=XMFLOAT4X4();
    XMMATRIX matrix=XMMatrixLookToLH(pos,look,up);
    XMStoreFloat4x4(&view,matrix);
    return view;
}

XMFLOAT4X4 Camera::GetProjMatrix() const {
    XMFLOAT4X4 proj;
    XMMATRIX P=XMMatrixPerspectiveFovLH(mFOV,mAspect,mNearZ,mFarZ);
    XMStoreFloat4x4(&proj,P);

    return proj;
}

XMFLOAT4X4 Camera::GetOrthoMatrix() const {
    XMFLOAT4X4 ortho;
    XMVECTOR dir=XMLoadFloat3(&(mLookAt));
    XMVECTOR result=XMVector3Dot(dir,dir);
    float temp;
    XMStoreFloat(&temp,result);
    float x=mLookAt.x;
    float z=mLookAt.z;
    if(abs(x)>=abs(z)){
        std::swap(x,z);
    }
    float w=50.0f*(abs(x)-((x*x)/abs(z))+(sqrt(x*x+z*z)/abs(z)));
    float h=(abs(mLookAt.y)/sqrt(temp))*w;
    std::cout<<"calc w&h:"<<w<<","<<h<<std::endl;
    XMMATRIX O=XMMatrixOrthographicLH(w,h,1.0f,1000.0F);
    XMStoreFloat4x4(&ortho,O);

    return ortho;
}

XMFLOAT4X4 Camera::GetViewProjMatrix() const {
    XMMATRIX view,proj;
    XMFLOAT4X4 v=GetViewMatrix();
    XMFLOAT4X4 p;
    if(mOrtho){
        p=GetOrthoMatrix();
    }else{
        p=GetProjMatrix();
    }
    view=XMLoadFloat4x4(&v);
    proj=XMLoadFloat4x4(&p);
    XMFLOAT4X4 result;
    XMMATRIX r=view*proj;
    XMStoreFloat4x4(&result,r);
    return result;
}

std::string Camera::GetRenderTarget() const {
    return mRenderTarget;
}

void Camera::SetDirection(XMFLOAT3 up, XMFLOAT3 lookat) {

    XMVECTOR u=XMVector3Normalize(XMLoadFloat3(&up));
    XMVECTOR l=XMVector3Normalize(XMLoadFloat3(&lookat));
    XMVECTOR r=XMVector3Normalize(XMVector3Cross(u,l));
    std::cout<<"origin lookat:"<<l<<", origin up:"<<u<<",origin right:"<<r<<std::endl;
    u=XMVector3Normalize(XMVector3Cross(l,r));
    XMStoreFloat3(&mRight,r);
    XMStoreFloat3(&mUp,u);
    XMStoreFloat3(&mLookAt,l);
    std::cout<<"LOOKAT:"<<l<<",up:"<<u<<",right:"<<r<<std::endl;
    UpdateValue();
}

void Camera::SetRenderTarget(const std::string &rtv) {
    mRenderTarget=rtv;
}
