// +--------------------------------------------+
// | File: material.h                           |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-06.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <QVector3D>
#include <QOpenGLShaderProgram>

namespace engine
{

struct MaterialUniformPack
{
    GLint mKaLoc;  // Ambient component uniform loc.
    GLint mKdLoc;  // Diffuse component uniform loc.
    GLint mKsLoc;  // Specular component uniform loc.
};

struct Material
{
    QVector3D mKa;  // Ambient component.
    QVector3D mKd;  // Diffuse component.
    QVector3D mKs;  // Specular component.
};

}  // namespace gloo.