#include "terrain.h"

#include <fstream>
#include <iostream>

#include "renderer/phong_renderer.h"

namespace engine
{

Terrain::Terrain()
{
    mGeometry.mScale = 1.0f;
    mGeometry.mTexScale = 1.0f;
    mGeometry.mHeightScale = 1.0f;

    mMaterial.mKd = { 0.8f, 0.8f, 0.8f };
    mMaterial.mKs = { 0.0f, 0.0f, 0.0f };
    mMaterial.mKa = { 0.0f, 0.0f, 0.0f };
    mMaterial.mOpacity = 1.0f;
    mMaterial.mShininess = 1.0f;
    mMaterial.mIllum = 2;
}

Terrain::~Terrain()
{
    for (int i = 0; i < mColorMapList.size(); i++)
    {
        delete mColorMapList[i];
    }

    for (int i = 0; i < mNormalMapList.size(); i++)
    {
        delete mNormalMapList[i];
    }

    int w = mChunkGrid.size();
    for (int u = 0; u < w; u++)
    {
        int h = mChunkGrid[u].size();

        for (int v = 0; v < h; v++)
        {
            delete mChunkGrid[u][v];
        }
    }
}

void Terrain::Render(PhongRenderer* renderer) const
{
    renderer->DisableNormalMap();
    renderer->SetMaterial(mMaterial);
    //renderer->DisableColorMap();
    renderer->EnableColorMap();

    mColorMapList[0]->bind(0);

    int w = mChunkGrid.size();
    for (int u = 0; u < w; u++)
    {
        int h = mChunkGrid[u].size();
        for (int v = 0; v < h; v++)
        {
            mChunkGrid[u][v]->Render(renderer);
        }
    }
}

bool Terrain::LoadFromR4T(const std::string & terrainFilePath)
{
    // Find last '/' or '\\' to retrieve containing folder.
    std::string folderPath = ".";
    size_t slash_pos = terrainFilePath.find_last_of("\\/");
    if (slash_pos != std::string::npos)  // Found.
        folderPath = terrainFilePath.substr(0, slash_pos);
    
    // Open .r4t file.
    std::ifstream r4tFile(terrainFilePath);

    if (!r4tFile.is_open())
    {
        std::cerr << "ERROR Could not open terrain file at \'" 
                  << terrainFilePath << "\'" << std::endl;
        return false;
    }

    // Read heightmap path.
    std::string heightmapPath;
    std::getline(r4tFile, heightmapPath);
    heightmapPath = folderPath + "/" + heightmapPath;

    // Load heightmap.
    mGeometry.mImage = QImage(QString::fromStdString(heightmapPath)).mirrored();

    if (mGeometry.mImage.isNull())
    {
        std::cerr << "ERROR Could not open heightmap at '" << heightmapPath << "'" << std::endl;
        return false;
    }

    // Read scaling factors (horizontal and vertical).
    r4tFile >> mGeometry.mScale >> mGeometry.mHeightScale;

    // Read grid dimensions and resize grid of terrain chunks.
    int w = 0, h = 0;
    r4tFile >> w >> h;
    mChunkGrid.resize(w, std::vector<TerrainChunk*>(h, nullptr));

    // Read texture scaling.
    r4tFile >> mGeometry.mTexScale;

    // Read list of texture paths.
    int numTextures = 0;
    r4tFile >> numTextures;

    mColorMapList.resize(numTextures, nullptr);
    mNormalMapList.resize(numTextures, nullptr);

    for (int i = 0; i < numTextures; i++)
    {
        // Read flag 'hasNormalMap'.
        int hasNormalMap = 0;
        r4tFile >> hasNormalMap;

        // Read color map.
        std::string relativePath;
        r4tFile >> relativePath;

        // Load color map.
        std::string texturePath = folderPath + "/" + relativePath;
        QImage colorMap = QImage(QString::fromStdString(texturePath)).mirrored();
        
        if (colorMap.isNull())
        {
            std::cerr << "WARNING Could not load color map at '" << texturePath << "'" << std::endl;
        }
        else
        {
            mColorMapList[i] = new QOpenGLTexture(colorMap);
            mColorMapList[i]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            mColorMapList[i]->setMagnificationFilter(QOpenGLTexture::Linear);
        }   

        // Read and load normal map.
        if (hasNormalMap)
        {
            r4tFile >> relativePath;
            folderPath + "/" + relativePath;
            QImage normalMap = QImage(QString::fromStdString(texturePath)).mirrored();

            if (normalMap.isNull())
            {
                std::cerr << "WARNING Could not load normal map at '" << texturePath << "'" << std::endl;
            }
            else
            {
                mNormalMapList[i] = new QOpenGLTexture(normalMap);
                mNormalMapList[i]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
                mNormalMapList[i]->setMagnificationFilter(QOpenGLTexture::Linear);
            } 
        }
    }

    int img_w = mGeometry.Width();
    int img_h = mGeometry.Height();

    int chunk_w = img_w / w;
    int chunk_h = img_h / h;

    // Temporary.
    mGeometry.mOrigin.setX(- img_w * mGeometry.mScale/2.0f);
    mGeometry.mOrigin.setY(0.0f);
    mGeometry.mOrigin.setZ(- img_h * mGeometry.mScale/2.0f);

    for (int u = 0; u < w; u++)
    {
        for (int v = 0; v < h; v++)
        {
            // Specify a window on the heightmap for building up the chunk.
            int xo = chunk_w * u;
            int yo = chunk_h * v;

            // The last chunk always fills until the border.
            int cw = (u < w-1) ? chunk_w+1 : img_w-xo;
            int ch = (v < h-1) ? chunk_h+1 : img_h-yo;

            mChunkGrid[u][v] = new TerrainChunk();
            mChunkGrid[u][v]->Load(mGeometry, xo, yo, cw, ch);
        }
    }

    return true;
}

}  // namespace engine.