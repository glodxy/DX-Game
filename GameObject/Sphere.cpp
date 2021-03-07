//
// Created by 田淙宇 on 2019/12/17.
//

#include "Sphere.h"
#include "../Component/Mesh.h"

using namespace GRCore;

Sphere::Sphere():GameObject(){
    Mesh* mesh=new Mesh(DEFAULT_SPHERE_MESH_NAME);
    mesh->SetShaderName(DEFAULT_SHADER_NAME);
    AddComponent(mesh);
}