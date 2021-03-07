//
// Created by 田淙宇 on 2019/12/9.
//

#include "Plane.h"
#include "../Component/Mesh.h"

using namespace GRCore;

Plane::Plane():GameObject(){
    Mesh* mesh=new Mesh(DEFAULT_PLANE_MESH_NAME);
    mesh->SetShaderName(DEFAULT_SHADER_NAME);
    AddComponent(mesh);
}