// +--------------------------------------------+
// | File: mtlb_file.h                          |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-12.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

namespace engine
{

const int kMTL_NAME_SIZE = 64;
const int kTEX_PATH_SIZE = 128;

// --------------------------------
// <MTLB Header>
// <material_1 header>
// <material_1 list of textures>
// ...
// <material_N header>
// <material_N list of textures>
// --------------------------------

// <list of textures>
// <size> <name>
// ...
// <size> <name>

struct MTLB_Header
{
    char _mtl_name[kMTL_NAME_SIZE];
    int  _num_materials;
};

struct MATERIAL_Header
{
    float _Ka[3];           // Ambient component.
    float _Kd[3];           // Diffuse component.
    float _Ks[3];           // Specular component.
    float _Ns;              // Shininess component.
    float _d;               // Transparency.
    int   _illum;           // Illumination type.
    int   _num_textures;    // Number of textures.
};

// If you want to set a texture to be null,
// please set its size to be 0.
// MATERIAL Maps order:
// map_Kd.      [color]
// map_Ks.      [specular]
// map_bump.    [normal/bump]
// map_d.       [transparency]
// ... as of 03/12.

}  // namespace engine.