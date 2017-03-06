#include "trackball_camera.h"

#include <QVector4D>

namespace engine
{

QVector3D TrackballCamera::GetCenter()
{
    QVector4D C = mView.inverted() * QVector4D(0.0f, 0.0f, 0.0f, 1.0f);
    return C.toVector3D();
}

void TrackballCamera::UpdateProjMatrix(int xo, int yo, int w, int h)
{
    mProjParameters.mAspect = static_cast<float>(w - xo) / static_cast<float>(h - yo);
    mProj.setToIdentity();

    const float & mFovy = mProjParameters.mFovy;
    const float & mFarZ = mProjParameters.mFarZ;
    const float & mNearZ = mProjParameters.mNearZ;
    const float & mAspect = mProjParameters.mAspect;

    mProj.perspective(mFovy, mAspect, mNearZ, mFarZ);
}

void TrackballCamera::UpdateViewMatrix()
{
    mView.setToIdentity();

    // V =  (S R T)^-1 = T^-1 R^-1 S^-1.
    mView.translate(0.0f, 0.0f, -mRadius);  // 5. Set distance to target.
    mView.rotate(-mPhi,   1, 0, 0);         // 4. Rotate x axis.
    mView.rotate(-mTheta, 0, 1, 0);         // 3. Rotate y axis.
    mView.scale(mScale);                    // 2. Scale.
    mView.translate(-mTarget);              // 1. Change center of the world.
}

void TrackballCamera::Rotate(const QPoint & mouseSpeed, float sensibility)
{
    TrackballCamera::RotateTheta(mouseSpeed.x() * sensibility);
    TrackballCamera::RotatePhi(mouseSpeed.y() * sensibility);
}

void TrackballCamera::Dolly(const QPoint & mouseSpeed, float sensibility)
{
    TrackballCamera::Dolly(sensibility * mouseSpeed.y());
}

QVector3D TrackballCamera::ComputeRayAt(float x_v, float y_v, float w, float h) const
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