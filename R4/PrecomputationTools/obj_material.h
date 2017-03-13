// +--------------------------------------------+
// | File: obj_material.h                       |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-11.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <string>
#include <QVector3D>

namespace obj_tool
{

namespace atomic
{

}  // namespace atomic.

// No support for transparency map, ambient map.
struct ObjMaterial
{
    ObjMaterial()
    : mNs(0.0f), mD(0.0f), mIllum(2)
    { }

    std::string mName;  // Material name.

    QVector3D mKa;  // Ambient component.
    QVector3D mKd;  // Diffuse component.
    QVector3D mKs;  // Specular component.
    float mNs;      // Shininess component.
    float mD;       // Transparency.
    int mIllum;     // Type of illumination.

    std::string mMapKd;  // Color map path.
    std::string mMapKs;  // Specular map path.
    std::string mMapBump;  // Normal/Bump map.
    std::string mMapD;     // Transparency map.
};



}  // namespace obj_tool.