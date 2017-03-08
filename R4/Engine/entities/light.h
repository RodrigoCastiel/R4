// +--------------------------------------------+
// | File: light.h                              |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-06.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <QVector3D>
#include <QOpenGLContext>

namespace engine
{

const GLint kNoUniform = -1;

struct LightUniformPack
{
    GLint mPosLoc;    // Location of vec3 position.
    GLint mDirLoc;    // Location of vec3 direction.
    GLint mLdLoc;     // Location of diffuse component.
    GLint mLsLoc;     // Location of specular component.
    GLint mAlphaLoc;  // Location of alpha component.
};

struct LightSource
{
    // Position and direction (in world coordinates).
    QVector3D mPos;
    QVector3D mDir;

    // Phong illumination model data.
    QVector3D mLd;  // Diffuse component.
    QVector3D mLs;  // Specular component.
    GLfloat mAlpha; // Shininess.

    /* Methods */
    void SetPosition(const QVector3D & pos) { mPos = pos; }
    void SetDirection(const QVector3D & dir) { mDir = dir; }

    void SetDiffuse(  const QVector3D & diff) { mLd = diff; }
    void SetSpecular( const QVector3D & spec) { mLs = spec; }
    void SetShininess(float shininess) { mAlpha = shininess; }
};


}  // namespace engine.