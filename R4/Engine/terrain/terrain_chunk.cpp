#include "terrain_chunk.h"

#include <QImage>
#include <vector>
#include <iostream>

#include "renderer/phong_renderer.h"

namespace engine
{

TerrainChunk::TerrainChunk()
{
    mTerrainMesh = nullptr;
}

TerrainChunk::~TerrainChunk()
{
    if (mTerrainMesh)
        delete mTerrainMesh;
}

void TerrainChunk::Render(PhongRenderer* renderer) const
{
    mTerrainMesh->Render(0);
}

bool TerrainChunk::Load(const HeightmapGeometry & geometry, int xo, int yo, int w, int h)
{
    // First step: load heightmap.
    mWidth  = w;
    mHeight = h;

    mPositions.reserve(3*w*h);
    mUVs.reserve(      2*w*h);
    mIndices.reserve(2*(h-2)*w + 2*w + 2*(h-2));
    mNormals.resize(3*w*h, 0.0f);

    // Build up geometry.
    const int numVertices = w*h;
    const int numElements = 2*(h-2)*w + 2*w + 2*(h-2);
    const GLenum drawMode = GL_TRIANGLE_STRIP;

    // Calculate vertices.
    for (int v = 0; v < h; v++) 
    {
        for (int u = 0; u < w; u++) 
        {
            QVector3D pos = geometry.PositionAt(xo+u, yo+v);
            float tex_u = (xo+u) / geometry.mTexScale;
            float tex_v = (yo+v) / geometry.mTexScale;

            mPositions.push_back(pos.x());
            mPositions.push_back(pos.y());
            mPositions.push_back(pos.z());

            mUVs.push_back(tex_u);
            mUVs.push_back(tex_v);
        }
    }

    // Calculate  normal vector.
    for (int y = 0; y < h; y++) 
    {
        for (int x = 0; x < w; x++) 
        {
            QVector3D n = geometry.NormalAt(x+xo, y+yo).normalized();
            float* normal = NormalAt(x, y);
            normal[0] = std::abs(n[0]);
            normal[1] = std::abs(n[1]);
            normal[2] = std::abs(n[2]);
        }
    }

    // Specify topology (triangles from elements).
    mIndices.reserve(numElements);
    for (int v = 0; v < h - 1; v++) 
    {
        for (int u = 0; u < w; u++) 
        {
            mIndices.push_back((v + 0)*w + u);
            mIndices.push_back((v + 1)*w + u);
        }

        // Triangle row transition: handle discontinuity.
        if (v < h - 2)
        {
            // Repeat last vertex and the next row first vertex to generate 
            // two invalid triangles and get continuity in the mesh.
            mIndices.push_back((v + 1)*w + (w - 1)); //INDEX(this->width-1, y+1);
            mIndices.push_back((v + 1)*w + 0);       //INDEX(0, y+1);
        }
    }

    TerrainChunk::UpdateMesh();

    return true;
}

void TerrainChunk::UpdateMesh()
{
    if (!mTerrainMesh) 
    {
        int num_pixels = mWidth * mHeight; 

        // Allocate mesh.
        mTerrainMesh = new MeshGroup(num_pixels, mIndices.size(), GL_TRIANGLE_STRIP);

        // Specify its attributes.  [POS NOR UV]. TODO: tangents!!
        mTerrainMesh->SetVertexAttribList({ 3, 3, 2 });

        // Add rendering pass for main rendering.
        mTerrainMesh->AddRenderingPass({ {0, true}, 
                                         {1, true},
                                         {2, true}});
        // ... and for depth rendering.
        // TODO. Probably just positions.

        // Load data.
        mTerrainMesh->Load({ mPositions.data(), mNormals.data(), mUVs.data()}, mIndices.data());
    }
    else
    {
        // UPDATE! (TODO).
    }
}

}  // namespace engine.