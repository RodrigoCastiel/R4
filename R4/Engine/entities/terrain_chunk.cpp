#include "terrain_chunk.h"

#include <QImage>
#include <vector>
#include <iostream>

#include "renderer/phong_renderer.h"

namespace engine
{



TerrainChunk::TerrainChunk(float scale, float heightScale)
    : mScale(scale)
    , mHeightScale(heightScale)
{
    mTerrainMesh = nullptr;

    mMaterial.mKa = {0.1f, 0.1f, 0.1f};
    mMaterial.mKd = {0.8f, 0.8f, 0.8f};
    mMaterial.mKs = {0.01f, 0.01f, 0.01f};

    mMaterial.mOpacity = 1.0f;
    mMaterial.mIllum = 2;
    mMaterial.mShininess = 1.0f;
}

TerrainChunk::~TerrainChunk()
{
    if (mTerrainMesh)
        delete mTerrainMesh;
}

void TerrainChunk::Render(PhongRenderer* renderer) const
{
    renderer->SetMaterial(mMaterial);
    mTerrainMesh->Render(0);
}

bool TerrainChunk::Load(const std::string & heightmapPath, const std::string & texture1Path)
{
    // First step: load heightmap.
    mHeightmap = QImage(QString::fromStdString(heightmapPath));

    if (mHeightmap.isNull())
    {
        std::cerr << "ERROR Could not load heightmap at '" << heightmapPath << "'" << std::endl;
        return false;
    }

    //textureList[j] = new QOpenGLTexture(img);
    //textureList[j]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    //textureList[j]->setMagnificationFilter(QOpenGLTexture::Linear);

    const int w = mHeightmap.size().width();
    const int h = mHeightmap.size().height();

    mPositions.reserve(3*w*h);
    mUVs.reserve(      2*w*h);
    mIndices.reserve(2*(h-2)*w + 2*w + 2*(h-2));
    mNormals.resize(3*w*h, 0.0f);

    // Build up geometry.
    const int numVertices = w*h;
    const int numElements = 2*(h-2)*w + 2*w + 2*(h-2);
    const GLenum drawMode = GL_TRIANGLE_STRIP;

    float xc = mScale * (w/2.0f);
    float zc = mScale * (h/2.0f);

    // Calculate vertices.
    for (int v = 0; v < h; v++) 
    {
        for (int u = 0; u < w; u++) 
        {
            QColor color = mHeightmap.pixel(u, v);

            float xp = u*mScale - xc;
            float yp = color.valueF() * mHeightScale;
            float zp = v*mScale - zc;

            mPositions.push_back(xp);
            mPositions.push_back(yp);
            mPositions.push_back(zp);

            mUVs.push_back(xp);
            mUVs.push_back(yp);
        }
    }

    // Calculate  normal vector.
    for (int v = 1; v < h - 1; v++) 
    {
        for (int u = 1; u < w - 1; u++) 
        {
            float* top_f = PositionAt(u, v-1);
            float* lft_f = PositionAt(u-1, v);
            float* rgt_f = PositionAt(u+1, v);
            float* bot_f = PositionAt(u, v+1);
            float* cen_f = PositionAt(u, v);

            QVector3D top(top_f[0], top_f[1], top_f[2]); // = pointsList[u][v - 1];
            QVector3D lft(lft_f[0], lft_f[1], lft_f[2]); // = pointsList[u - 1][v];
            QVector3D rgt(rgt_f[0], rgt_f[1], rgt_f[2]); // = pointsList[u + 1][v];
            QVector3D bot(bot_f[0], bot_f[1], bot_f[2]); // = pointsList[u][v + 1];
            QVector3D cen(cen_f[0], cen_f[1], cen_f[2]); // = pointsList[u][v];

            QVector3D n = QVector3D::crossProduct(rgt - cen, top - cen)
                        + QVector3D::crossProduct(top - cen, lft - cen)
                        + QVector3D::crossProduct(lft - cen, bot - cen)
                        + QVector3D::crossProduct(bot - cen, rgt - cen);
            n.normalize();

            mNormals[3*(w*v + u) + 0] = std::abs(n[0]);
            mNormals[3*(w*v + u) + 1] = std::abs(n[1]);
            mNormals[3*(w*v + u) + 2] = std::abs(n[2]);
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
        int num_pixels = mHeightmap.size().width() * mHeightmap.size().height(); 

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