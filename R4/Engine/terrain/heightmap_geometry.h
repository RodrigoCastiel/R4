// +--------------------------------------------+
// | File: terrain.h                            |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-18.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <QImage>
#include <QVector3D>

namespace engine
{

struct HeightmapGeometry
{
    /* Attributes */
    QImage mImage;  // Heightmap image.

    QVector3D mOrigin;    // Position at (u, v).
    float mScale;         // Horizontal scaling factor.
    float mTexScale;      // Texture scaling factor.
    float mHeightScale;   // Vertical scaling factor.
    
    /* Methods */

    // Img dimensions.
    int Width() const { return mImage.width(); }
    int Height() const { return mImage.height(); }

    // Returns y coordinate at pixel (u, v).
    float HeightAt(int u, int v) const;

    // Returns height at point (x, ?, z).
    // If outside, it return clamps to the image limits.
    // Uses bilinear interpolation.
    float HeightAt(float xc, float zc) const;

    // Returns point at pixel (u, v).
    // u in [0, width) and v in [0, height).
    QVector3D PositionAt(int u, int v) const;

    // Returns normal vector at pixel (u, v).
    // Does not normalize.
    QVector3D NormalAt(int u, int v) const;
};

}  // namespace engine.