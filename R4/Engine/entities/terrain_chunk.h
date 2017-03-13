// +--------------------------------------------+
// | File: terrain_chunk_mesh.h                 |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-07.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <QOpenGLTexture>

#include "core/gl_core.h" 

#include "mesh/material_lib.h"
#include "mesh/mesh_group.h"

namespace engine
{

// TerrainChunk represents a terrain slice, where Y is the height.
class TerrainChunk
{
public:
    // +++ Constructors +++ ---------------------------------------------------
    TerrainChunk(int resX, int resZ, float scale = 1.0f);
    ~TerrainChunk();

    // +++ Initialization +++ -------------------------------------------------
    
    // Builds up all geometry.
    bool Load();

    void UpdateMesh();

    // +++ Getters/Setters +++ ------------------------------------------------

    // Sets main texture. It will not own it.
    void SetTexture(QOpenGLTexture* texture) { mTexture = texture; }

    // Sets all attributes for main rendering shaders.
    void SetMainRendererAttribList(GLint posAttr, GLint normAttr, GLint uvAttr);

    MeshGroup* GetMeshGroup() { return mTerrainMesh; }
    const Material & GetMaterial() { return mMaterial; }
    QOpenGLTexture* GetTexture() { return mTexture; }

    float* PositionAt(int i_x, int j_z);
    float* NormalAt(int i_x, int j_z);
    float* UvAt(int i_x, int j_z);

private:
    // Terrain geometry.
    int mResolutionX;   // Number of vertices along x.
    int mResolutionZ;   // Number of vertices along z.
    float mScale;       // Scale of a single tile.
    
    std::vector<float> mPositions;  // Vertex positions.
    std::vector<float> mNormals;    // Vertex normals.
    std::vector<float> mUVs;        // Vertex UV coordinates.
    std::vector<GLuint> mIndices;   // Topology elements for GL_TRIANGLE_STRIP.

    // Renderable mesh.
    MeshGroup* mTerrainMesh;
    GLint mMainRendererAttribList[4];   // Attribute list for main renderer.
    GLint mDepthRendererAttribList[4];  // Attribute list for depth renderer.

    Material mMaterial;

    // Main texture.
    QOpenGLTexture* mTexture;
};

// === INLINE METHODS ===

inline
float* TerrainChunk::PositionAt(int i_x, int j_z)
{
    return &mPositions[3*(mResolutionX*j_z + i_x)];
}

inline
float* TerrainChunk::NormalAt(int i_x, int j_z)
{
    return &mNormals[3*(mResolutionX*j_z + i_x)];
}

inline
float* TerrainChunk::UvAt(int i_x, int j_z)
{
    return &mUVs[2*(mResolutionX*j_z + i_x)];
}


}  // namespace engine.