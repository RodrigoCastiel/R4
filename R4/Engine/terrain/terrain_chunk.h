// +--------------------------------------------+
// | File: terrain_chunk_mesh.h                 |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-07.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <QImage>
#include <QOpenGLTexture>

#include "core/gl_core.h" 

#include "mesh/mesh_group.h"
#include "mesh/material_lib.h"

#include "heightmap_geometry.h"

namespace engine
{

// TerrainChunk represents a terrain slice, where Y is the height.
class TerrainChunk
{
public:
    // +++ Constructors +++ ---------------------------------------------------
    TerrainChunk();
    ~TerrainChunk();

    // +++ Initialization +++ -------------------------------------------------
    
    // Builds up all geometry.
    bool Load(const HeightmapGeometry & geometry, int xo, int yo, int w, int h);

    void UpdateMesh();

    // +++ Rendering +++ ------------------------------------------------------

    void Render(class PhongRenderer* renderer) const;

    // +++ Getters/Setters +++ ------------------------------------------------
    
    MeshGroup* GetMeshGroup() { return mTerrainMesh; }

    float* PositionAt(int u, int v);
    float* NormalAt(int u, int v);
    float* UvAt(int u, int v);
    //float* TangentAt(int u, int v);

private:
    std::vector<float> mPositions;  // Vertex positions.
    std::vector<float> mNormals;    // Vertex normals.
    std::vector<float> mUVs;        // Vertex UV coordinates.
    // std::vector<float> mTangents;  // Vertex tangents.
    std::vector<GLuint> mIndices;   // Topology elements for GL_TRIANGLE_STRIP.

    // Renderable mesh.
    MeshGroup* mTerrainMesh;

    int mWidth;  // Along u.
    int mHeight; // Along v.
};

// === INLINE METHODS ===

inline
float* TerrainChunk::PositionAt(int u, int v)
{
    return &mPositions[3*(mWidth*v + u)];
}

inline
float* TerrainChunk::NormalAt(int u, int v)
{
    return &mNormals[3*(mWidth*v + u)];
}

inline
float* TerrainChunk::UvAt(int u, int v)
{
    return &mUVs[2*(mWidth*v + u)];
}


}  // namespace engine.