// +--------------------------------------------+
// | File: static_object.h                      |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-13.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include "core/gl_core.h"

#include "mesh/mesh_group.h"
#include "mesh/material_lib.h"

namespace engine
{

class StaticObject
{
public:
    // +++ Constructors/Destructors +++ ---------------------------------------
    StaticObject();
    ~StaticObject();

    // +++ Loading/Setup +++ --------------------------------------------------

    bool LoadFromR4O(const std::string & filePath);

    // +++ Main / Rendering +++ -----------------------------------------------

    inline std::vector<MeshGroup*> & Groups() { return mGroups; }
    inline const std::vector<MeshGroup*> & Groups() const { return mGroups; }

private:
    // Attributes.
    MaterialLib mMaterialLib;         // MTLLIB.
    std::vector<MeshGroup*> mGroups;  // Mesh groups.
    //std::vector<int> ;
    
};



}  // namespace engine.