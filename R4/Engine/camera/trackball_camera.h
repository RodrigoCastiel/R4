// +--------------------------------------------+
// | File: trackball_camera.h                   |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-06.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <QMatrix4x4>

#include "projection_parameters.h"

namespace engine
{

class TrackballCamera
{
public:
    // +++ Constructors/Destructor +++ ------------------------------------------------------------
    TrackballCamera() { }
    TrackballCamera(ProjectionParameters && projParameters)
        : mProjParameters(projParameters) { }
    TrackballCamera(const ProjectionParameters & projParameters)
        : mProjParameters(projParameters) { }

    ~TrackballCamera() { }

    // +++ Main +++ -------------------------------------------------------------------------------

    void UpdateProjMatrix(int xo, int yo, int w, int h);
    void UpdateViewMatrix();

    // Set projection parameters.
    void SetProjectionParameters(const ProjectionParameters & projParameters);

    inline const QMatrix4x4 & GetProjMatrix() const { return mProj; }
    inline const QMatrix4x4 & GetViewMatrix() const { return mView; }

    // Returns proj*view. You must update the matrices before!
    QMatrix4x4 GetProjView() { return (mProj * mView); }

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

    QVector3D GetCenter();

    const ProjectionParameters & GetProjectionParameters() const { return mProjParameters; }
    ProjectionParameters & GetProjectionParameters() { return mProjParameters; }

    // +++ Geometry +++ ---------------------------------------------------------------------------

    // Computes the vector which goes from camera center to mouse coordinates on projection plane.
    QVector3D ComputeRayAt(float x_v, float y_v, float w, float h) const;

protected:
    float mPhi    { 0.0f };  // Phi (spherical coordinates).
    float mTheta  { 0.0f };  // Theta (spherical coordinates).
    float mRadius { 1.0f };  // Radius (spherical coordinates).
    QVector3D mTarget { 0.0f, 0.0f, 0.0f };  // Target point.
    QVector3D mScale  { 1.0f, 1.0f, 1.0f };  // Scaling factors for x, y, z axis.

    QMatrix4x4 mView;  // Specifies camera position, orientation and so on [a stack].
    QMatrix4x4 mProj;  // Specifies projective transform.
    ProjectionParameters mProjParameters;  // Specifies parameters of projection.
};

inline
void TrackballCamera::SetProjectionParameters(const ProjectionParameters & projParameters)
{
    mProjParameters = projParameters;
}

}  // namespace engine.