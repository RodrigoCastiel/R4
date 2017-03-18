#include "heightmap_geometry.h"

namespace engine
{


float HeightmapGeometry::HeightAt(int u, int v) const
{
    // Clamp pixel coordinates.
    u = std::max(u, 0);
    u = std::min(u, mImage.width()-1);
    v = std::max(v, 0);
    v = std::min(v, mImage.height()-1);
    QColor color = mImage.pixel(u, v);
    return color.valueF() * mHeightScale;
}

float HeightmapGeometry::HeightAt(float xc, float zc) const
{
    // Bottom left pixel.
    int uo = static_cast<int>((xc - mOrigin.x())/mScale);
    int vo = static_cast<int>((zc - mOrigin.z())/mScale);

    // Bottom-left and top-right (x, z) coordinates.
    float xo = uo*mScale + mOrigin.x();
    float zo = vo*mScale + mOrigin.z();
    float x = xo + mScale;
    float z = zo + mScale;
    
    // Interpolate x.
    float alpha = 1.0f - (x - xc)/mScale;
    float y_zo = (1-alpha)*HeightAt(uo, vo) + alpha*HeightAt(uo+1, vo);
    float y_z  = (1-alpha)*HeightAt(uo, vo+1) + alpha*HeightAt(uo+1, vo+1);

    // And then, zc.
    float beta = 1.0f - (z - zc)/mScale;
    float yc = (1-beta)*y_zo + beta*y_z;

    return yc;
}

QVector3D HeightmapGeometry::PositionAt(int u, int v) const
{
    // Clamp pixel coordinates.
    u = std::max(u, 0);
    u = std::min(u, mImage.width()-1);
    v = std::max(v, 0);
    v = std::min(v, mImage.height()-1);

    QColor color = mImage.pixel(u, v);
    QVector3D position = mOrigin;
    position[0] += u*mScale;
    position[1] += color.valueF() * mHeightScale;
    position[2] += v*mScale;
    return position;
}

QVector3D HeightmapGeometry::NormalAt(int u, int v) const
{
    QVector3D top = PositionAt(u, v-1);
    QVector3D lft = PositionAt(u-1, v);
    QVector3D rgt = PositionAt(u+1, v);
    QVector3D bot = PositionAt(u, v+1);
    QVector3D cen = PositionAt(u, v);

    QVector3D n = QVector3D::crossProduct(rgt - cen, top - cen)
                + QVector3D::crossProduct(top - cen, lft - cen)
                + QVector3D::crossProduct(lft - cen, bot - cen)
                + QVector3D::crossProduct(bot - cen, rgt - cen);
    return n;
}

}  // namespace engine.