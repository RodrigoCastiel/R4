// +--------------------------------------------+
// | File: terrain.h                            |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-16.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <vector>

#include "mesh/material_lib.h"
#include "terrain/terrain_chunk.h"
#include "terrain/heightmap_geometry.h"

namespace engine
{

// ----------------------------------------------
// .r4t >> R4 Terrain file.
//
// Format:
//  <heightmap path>
//  <scale> <vertical scale>
//  <grid w> <grid h>
//  <texture coord scale>
//  <num textures>
//  <bool has normal map?>
//    <color map path>
//    [normal map path]          if flag is true
//   ...
//
// ----------------------------------------------

class Terrain
{
public:
    // +++ Constructors/Destructor +++ ----------------------------------------
   
    // Default constructor.
    Terrain();

    ~Terrain();

    // +++ Loading/Rendering +++ ----------------------------------------------

    void Render(class PhongRenderer* renderer) const;

    // Loads from .r4t file (terrain file).
    bool LoadFromR4T(const std::string & terrainFilePath);

    const Material & GetMaterial() { return mMaterial; }

    // +++ Geometry / Collision +++ -------------------------------------------

    HeightmapGeometry & Geometry() { return mGeometry; }
    
private:
    HeightmapGeometry mGeometry;

    // Matrix of terrain 'slices' (square matrix).
    std::vector<std::vector<TerrainChunk*>> mChunkGrid;

    // Set of textures.
    std::vector<QOpenGLTexture*> mColorMapList;
    std::vector<QOpenGLTexture*> mNormalMapList;

    // Material.
    Material mMaterial;
};


}  // namespace engine.