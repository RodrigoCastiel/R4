#include "trackball_camera.h"

#include <QVector4D>

namespace engine
{

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

}  // namespace engine.