#include "terrain_chunk.h"

#include <QImage>
#include <vector>

namespace engine
{



TerrainChunk::TerrainChunk(int resX, int resZ, float scale)
    : mResolutionX(resX)
    , mResolutionZ(resZ)
    , mScale(scale)
{
    mTexture = nullptr;
    mTerrainMesh = nullptr;

    const int w = mResolutionX;
    const int h = mResolutionZ;
    mPositions.reserve(3 * w*h);
    mNormals.reserve(  3 * w*h);
    mUVs.reserve(      2 * w*h);
    mIndices.reserve(2*(h-2)*w + 2*w + 2*(h-2));

    mMaterial.mKa = {0.1f, 0.1f, 0.1f};
    mMaterial.mKd = {0.9f, 0.9f, 0.9f};
    mMaterial.mKs = {0.1f, 0.1f, 0.1f};
}

TerrainChunk::~TerrainChunk()
{
    if (mTerrainMesh)
        delete mTerrainMesh;
}

void TerrainChunk::SetMainRendererAttribList(
    GLint posAttr, GLint normAttr, GLint uvAttr)
{
    mMainRendererAttribList[0] = posAttr;
    mMainRendererAttribList[1] = normAttr;
    mMainRendererAttribList[2] = uvAttr;
}

bool TerrainChunk::Load()
{
    const int w = mResolutionX;
    const int h = mResolutionZ;

    // Build up geometry.
    const int numVertices = w*h;
    const int numElements = 2*(h-2)*w + 2*w + 2*(h-2);
    const GLenum drawMode = GL_TRIANGLE_STRIP;

    float xc = mScale * (mResolutionX/2.0f);
    float zc = mScale * (mResolutionZ/2.0f);

    // Calculate vertices.
    for (int z = 0; z < h; z++) {
        for (int x = 0; x < w; x++) {
            float xp = x*mScale - xc;
            float yp = 0.0f;
            float zp = z*mScale - zc;

            mPositions.push_back(xp);
            mPositions.push_back(yp);
            mPositions.push_back(zp);

            mNormals.push_back(0.0);
            mNormals.push_back(1.0);
            mNormals.push_back(0.0);

            mUVs.push_back(x);
            mUVs.push_back(z);
        }
    }

    // Specify topology (triangles from elements).
    mIndices.reserve(numElements);
    for (int v = 0; v < h - 1; v++) {
        for (int u = 0; u < w; u++) {
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
        // Allocate mesh.
        mTerrainMesh = new MeshGroup(mResolutionX*mResolutionZ, mIndices.size(), GL_TRIANGLE_STRIP);

        // Specify its attributes.  [POS NOR UV]
        mTerrainMesh->SetVertexAttribList({ 3, 3, 2 });

        // Add rendering pass for main rendering.
        mTerrainMesh->AddRenderingPass({ {mMainRendererAttribList[0], true}, 
                                         {mMainRendererAttribList[1], true},
                                         {mMainRendererAttribList[2], true}});
        // ... and for depth rendering.
        /*mTerrainMesh->AddRenderingPass({{mDepthRendererAttribList[0], true}, 
                                        {mMainRendererAttribList[1], true},
                                        {mMainRendererAttribList[2], true}});*/

        // Load data.
        mTerrainMesh->Load({ mPositions.data(), mNormals.data(), mUVs.data()}, mIndices.data());
    }
    else
    {
        // UPDATE! (TODO).
    }
}

}  // namespace engine.