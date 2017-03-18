#include "fpv_camera.h"

#include <QVector2D>

namespace engine
{
void FPVCamera::UpdateViewMatrix()
{
    mView.setToIdentity();
    mView.rotate(-mPitch,  1, 0, 0);       // 4. Rotate x axis.
    mView.rotate(-mYaw,    0, 1, 0);           // 3. Rotate y axis.
    mView.scale(mScale);                    // 2. Scale.
    mView.translate(-mCenter);              // 1. Translate.
}

void FPVCamera::MoveRight(float distance)
{
    float yaw_rad = (mYaw - 90.0f) * 3.141592 / 180.0;
    QVector3D direction;
    direction.setX(-std::sin(yaw_rad));
    direction.setY(0.0);
    direction.setZ(-std::cos(yaw_rad));
    mCenter += distance * direction.normalized();
}

void FPVCamera::MoveForward(float distance)
{
    float yaw_rad = mYaw * 3.141592 / 180.0;
    QVector3D direction;
    direction.setX(-std::sin(yaw_rad));
    direction.setY(0.0);
    direction.setZ(-std::cos(yaw_rad));
    mCenter += distance * direction.normalized();
}

void FPVCamera::MoveWASD(bool w, bool a, bool s, bool d, float distance)
{
    float forward = 0.0;
    float right = 0.0f;

    // Opposite keys cancel each other!

    if (w && !s)
        forward = 1.0f;
    else if (!w && s)
        forward = -1.0f;

    if (a && !d)
        right = -1.0f;
    else if (!a && d)
        right = 1.0f;

    float norm = std::sqrtf(forward*forward + right*right);
    if (norm > 1e-6)
    {
        forward /= norm;
        right /= norm;
        forward *= distance;
        right *= distance;

        FPVCamera::MoveForward(forward);
        FPVCamera::MoveRight(right);
    }
}

void FPVCamera::RotateYaw(float theta)
{
    mYaw += theta;

    if (mYaw > 180.0f)
        mYaw -= 360.0f;
    if (mYaw < -180.0f)
        mYaw += 360.0f;
}

void FPVCamera::RotatePitch(float theta)
{
    mPitch += theta;

    mPitch = std::min(90.0f, mPitch);
    mPitch = std::max(-90.0f, mPitch);
}

void FPVCamera::Rotate(const QPoint & mouseSpeed, float sensibility)
{
    FPVCamera::RotateYaw(-mouseSpeed.x() * sensibility);
    FPVCamera::RotatePitch(-mouseSpeed.y() * sensibility);

    printf("yaw = %lf\n", mYaw);
}

}  // namespace engine.