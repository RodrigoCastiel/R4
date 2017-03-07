#include "camera.h"

namespace engine
{

QVector3D Camera::GetCenter()
{
    QVector4D C = mView.inverted() * QVector4D(0.0f, 0.0f, 0.0f, 1.0f);
    return C.toVector3D();
}

void Camera::UpdateProjMatrix(int xo, int yo, int w, int h)
{
    mProjParameters.mAspect = static_cast<float>(w - xo) / static_cast<float>(h - yo);
    mProj.setToIdentity();

    const float & mFovy = mProjParameters.mFovy;
    const float & mFarZ = mProjParameters.mFarZ;
    const float & mNearZ = mProjParameters.mNearZ;
    const float & mAspect = mProjParameters.mAspect;

    mProj.perspective(mFovy, mAspect, mNearZ, mFarZ);
}

QVector3D Camera::ComputeRayAt(float x_v, float y_v, float w, float h) const
{
    // Compute boundaries of near clip plane.
    float x_max = 1.0f;
    float y_max = 1.0f;

    // Convert viewport coordinates to projection plane coordinates.
    y_v = (h - y_v);
    float xp = ((x_v / w) * (2 * x_max)) - x_max;
    float yp = ((y_v / h) * (2 * y_max)) - y_max;

    const QMatrix4x4 PV = mProj * mView;
    QVector4D ray = PV.inverted() * QVector4D(xp, yp, 1.0, 1.0);

    return ray.toVector3D();
}

}  // namespace engine.