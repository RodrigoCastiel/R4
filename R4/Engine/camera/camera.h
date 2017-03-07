// +--------------------------------------------+
// | File: camera.h                             |
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

class Camera
{
public:
    // +++ Constructors/Destructor +++ ------------------------------------------------------------
    Camera() { }
    Camera(ProjectionParameters && projParameters)
        : mProjParameters(projParameters) { }
    Camera(const ProjectionParameters & projParameters)
        : mProjParameters(projParameters) { }

    virtual ~Camera() { }

    // +++ Main +++ -------------------------------------------------------------------------------

    virtual void UpdateProjMatrix(int xo, int yo, int w, int h);
    virtual void UpdateViewMatrix() = 0;

    // Set projection parameters.
    virtual void SetProjectionParameters(const ProjectionParameters & projParameters);

    inline const QMatrix4x4 & GetProjMatrix() const { return mProj; }
    inline const QMatrix4x4 & GetViewMatrix() const { return mView; }

    // Returns proj*view. You must update the matrices before!
    QMatrix4x4 GetProjView() { return (mProj * mView); }

    // +++ Getter/Setters +++ ---------------------------------------------------------------------

    inline void SetProjMatrix(const QMatrix4x4 & proj) { mProj = proj; }
    inline void SetViewMatrix(const QMatrix4x4 & view) { mView = view; }

    virtual QVector3D GetCenter();

    const ProjectionParameters & GetProjectionParameters() const { return mProjParameters; }
    ProjectionParameters & GetProjectionParameters() { return mProjParameters; }

    // +++ Geometry +++ ---------------------------------------------------------------------------

    // Computes the vector which goes from camera center to mouse coordinates on projection plane.
    virtual QVector3D ComputeRayAt(float x_v, float y_v, float w, float h) const;

protected:
    QMatrix4x4 mView;  // Specifies camera position, orientation and so on [a stack].
    QMatrix4x4 mProj;  // Specifies projective transform.
    ProjectionParameters mProjParameters;  // Specifies parameters of projection.
};

inline
    void Camera::SetProjectionParameters(const ProjectionParameters & projParameters)
{
    mProjParameters = projParameters;
}

}  // namespace engine.