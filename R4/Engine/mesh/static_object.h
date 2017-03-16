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
    StaticObject() 
    {}
    
    ~StaticObject();

    // +++ Loading/Setup +++ --------------------------------------------------

    bool LoadFromR4O(const std::string & filePath);

    // +++ Main / Rendering +++ -----------------------------------------------

    void Render(class PhongRenderer* renderer) const;
    //void RenderDepthOnly();

    inline std::vector<MeshGroup*> & Groups() { return mGroups; }
    inline const std::vector<MeshGroup*> & Groups() const { return mGroups; }

    inline const MaterialLib & GetMaterialLib() const { return mMaterialLib; }

private:
    // Attributes.
    MaterialLib mMaterialLib;         // MTLLIB.
    std::vector<MeshGroup*> mGroups;  // Mesh groups.
};



}  // namespace engine.