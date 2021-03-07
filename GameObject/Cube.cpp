//
// Created by 田淙宇 on 2019/12/3.
//

#include "Cube.h"
#include "../Component/Mesh.h"

using namespace GRCore;
Cube::Cube():GameObject(){

    Mesh* mesh=new Mesh(DEFAULT_CUBE_MESH_NAME);
    mesh->SetShaderName(DEFAULT_SHADER_NAME);
    AddComponent(mesh);
}