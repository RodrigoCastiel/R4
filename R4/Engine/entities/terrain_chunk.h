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

#include "mesh/material_lib.h"
#include "mesh/mesh_group.h"

namespace engine
{

// TerrainChunk represents a terrain slice, where Y is the height.
class TerrainChunk
{
public:
    // +++ Constructors +++ ---------------------------------------------------
    TerrainChunk(float scale = 3.0f, float height_scale = 200.0f);
    ~TerrainChunk();

    // +++ Initialization +++ -------------------------------------------------
    
    // Builds up all geometry.
    bool Load(const std::string & heightmapPath, const std::string & texture1Path);

    void UpdateMesh();

    // +++ Rendering +++ ------------------------------------------------------

    void Render(class PhongRenderer* renderer) const;

    // +++ Getters/Setters +++ ------------------------------------------------

    MeshGroup* GetMeshGroup() { return mTerrainMesh; }
    const Material & GetMaterial() { return mMaterial; }
    
    QOpenGLTexture* GetTexture(int index) { return mTextureList[index]; }
    int GetNumTextures() const { return mTextureList.size(); }

    float* PositionAt(int u, int v);
    float* NormalAt(int u, int v);
    float* UvAt(int u, int v);
    //float* TangentAt(int u, int v);

private:
    // Terrain geometry.
    //int mResolutionX;   // Number of vertices along x.
    //int mResolutionZ;   // Number of vertices along z.
    float mScale;         // Scale of a single tile.
    float mHeightScale;   // Scale of y-axis.
    
    std::vector<float> mPositions;  // Vertex positions.
    std::vector<float> mNormals;    // Vertex normals.
    std::vector<float> mUVs;        // Vertex UV coordinates.
    // std::vector<float> mTangents;  // Vertex tangents.
    std::vector<GLuint> mIndices;   // Topology elements for GL_TRIANGLE_STRIP.

    // Renderable mesh.
    MeshGroup* mTerrainMesh;
    Material mMaterial;

    QImage mHeightmap;                          // Heightmap image.
    std::vector<QOpenGLTexture*> mTextureList;  // Set of textures.
};

// === INLINE METHODS ===

inline
float* TerrainChunk::PositionAt(int u, int v)
{
    const int w = mHeightmap.size().width();
    return &mPositions[3*(w*v + u)];
}

inline
float* TerrainChunk::NormalAt(int u, int v)
{
    const int w = mHeightmap.size().width();
    return &mNormals[3*(w*v + u)];
}

inline
float* TerrainChunk::UvAt(int u, int v)
{
    const int w = mHeightmap.size().width();
    return &mUVs[2*(w*v + u)];
}


}  // namespace engine.