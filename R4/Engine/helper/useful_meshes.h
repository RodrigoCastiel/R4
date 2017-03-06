// +--------------------------------------------+
// | File: useful_meshes.h                      |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-06.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <QVector3D>

#include "core/gl_core.h"
#include "mesh/mesh_group.h"

namespace engine
{

struct AxisMesh
{
public:
    AxisMesh(GLint positionAttribLoc, GLint colorAttribLoc);
    ~AxisMesh();

    void Render() const;
    void Update(GLfloat x, GLfloat y, GLfloat z);

    const MeshGroup* GetMeshGroup() const { return mMeshGroup; }

private:
    MeshGroup* mMeshGroup;
};

struct BoundingBoxMesh
{
public:
    BoundingBoxMesh(GLint positionAttribLoc, GLint colorAttribLoc,
                    const QVector3D & rgb = {1.0f, 1.0f, 1.0f});
    ~BoundingBoxMesh();

    void Render() const;
    void Update(GLfloat xmin, GLfloat xmax, GLfloat ymin, GLfloat ymax, GLfloat zmin, GLfloat zmax);

    const MeshGroup* GetMeshGroup() const { return mMeshGroup; }

private:
    MeshGroup* mMeshGroup;
};

struct GridMesh
{
    public:
    GridMesh(GLint positionAttribLoc, GLint colorAttribLoc, int width, int height, GLfloat tileSize = 1.0f,
            const QVector3D & rgb = {0.8f, 0.8f, 0.8f});
    ~GridMesh();

    void Render() const;

    const MeshGroup* GetMeshGroup() const { return mMeshGroup; }

private:
    MeshGroup* mMeshGroup;
};



}  // namespace engine.