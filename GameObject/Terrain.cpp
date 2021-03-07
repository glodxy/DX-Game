//
// Created by 田淙宇 on 2019/12/9.
//

#include "Terrain.h"
#include "../Component/Mesh.h"

using namespace GRCore;

Terrain::Terrain():GameObject(){
    Mesh* mesh=new Mesh(DEFAULT_TERRAIN_MESH_NAME);
    mesh->SetShaderName(DEFAULT_SHADER_NAME);
    AddComponent(mesh);
}
