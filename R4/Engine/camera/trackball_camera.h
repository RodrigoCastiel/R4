// +--------------------------------------------+
// | File: trackball_camera.h                   |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-06.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include "camera.h"

namespace engine
{

class TrackballCamera : public Camera
{
public:
    // +++ Constructors/Destructor +++ ------------------------------------------------------------
    TrackballCamera() { }
    TrackballCamera(ProjectionParameters && projParameters)
        : Camera(projParameters) { }
    TrackballCamera(const ProjectionParameters & projParameters)
        : Camera(projParameters) { }

    ~TrackballCamera() { }

    // +++ Main +++ -------------------------------------------------------------------------------

    void UpdateViewMatrix();

    // +++ Interaction +++ ------------------------------------------------------------------------

    void MoveTarget(const QVector3D & displacement) { mTarget += displacement; }

    void RotatePhi(float d_phi)     { mPhi += d_phi; }
    void RotateTheta(float d_theta) { mTheta += d_theta; }
    void Dolly(float d_radius) { mRadius += d_radius; }

    void Rotate(const QPoint & mouseSpeed, float sensibility = 0.25f);
    void Dolly (const QPoint & mouseSpeed, float sensibility = 0.01f);

    // +++ Getter/Setters +++ ---------------------------------------------------------------------

    inline void SetProjMatrix(const QMatrix4x4 & proj) { mProj = proj; }
    inline void SetViewMatrix(const QMatrix4x4 & view) { mView = view; }

    inline void SetDistanceToTarget(float dist) { mRadius = dist; }
    inline float GetDistanceToTarget() { return mRadius; }

    inline void SetTheta(float theta) { mTheta = theta; }
    inline float GetTheta() const { return mTheta; }
    
    inline void SetPhi(float phi) { mPhi = phi; }
    inline float GetPhi() const { return mPhi; }

    inline void SetTarget(const QVector3D & target) { mTarget = target; }
    const QVector3D & GetTarget() const { return mTarget; }

protected:
    float mPhi    { 0.0f };  // Phi (spherical coordinates).
    float mTheta  { 0.0f };  // Theta (spherical coordinates).
    float mRadius { 1.0f };  // Radius (spherical coordinates).
    QVector3D mTarget { 0.0f, 0.0f, 0.0f };  // Target point.
    QVector3D mScale  { 1.0f, 1.0f, 1.0f };  // Scaling factors for x, y, z axis.
};

}  // namespace engine.