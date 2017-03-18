// +--------------------------------------------+
// | File: fpv_camera.h                         |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-16.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+
#pragma once

#include "camera/camera.h"

namespace engine
{

class FPVCamera : public Camera
{
public:
    // +++ Constructors/Destructor +++ ------------------------------------------------------------
    FPVCamera() { }
    FPVCamera(ProjectionParameters && projParameters)
        : Camera(projParameters) { }
    FPVCamera(const ProjectionParameters & projParameters)
        : Camera(projParameters) { }

    ~FPVCamera() { }

    // +++ Main +++ -------------------------------------------------------------------------------

    void UpdateViewMatrix();

    // +++ Interaction +++ ------------------------------------------------------------------------

    // Translation.
    void MoveRight  (float distance);
    void MoveForward(float distance);
    
    void MoveWASD(bool w, bool a, bool s, bool d, float distance = 1.0f);

    // Rotation.
    void RotateYaw(float theta);
    void RotatePitch(float theta);

    void Rotate(const QPoint & mouseSpeed, float sensibility = 0.25f);

    // +++ Getter/Setters +++ ---------------------------------------------------------------------

    const QVector3D & GetCenter() const { return mCenter; };
    void SetCenter(QVector3D & center) { mCenter = center; }

protected:
    float mYaw    { 0.0f };
    float mPitch  { 0.0f };
    QVector3D mCenter    { 0.0f, 100.0f, 0.0f };  // Center.
    QVector3D mScale     { 1.0f, 1.0f, 1.0f };  // Scaling factors for x, y, z axis.
};


}  // namespace engine.