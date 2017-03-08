// +--------------------------------------------+
// | File: render_engine.h                      |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-05.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include "core/gl_core.h"

#include "camera/camera.h"

#include "helper/useful_meshes.h"

#include "debug_renderer.h"
#include "phong_renderer.h"

#include <QOpenGLTexture>

namespace engine
{

class RenderEngine
{
public:
    // +++ Constructor/Destructor +++ -----------------------------------------
    RenderEngine();
    ~RenderEngine();

    // +++ Rendering Queues +++ -----------------------------------------------

    // +++ Main +++ -----------------------------------------------------------

    bool Load();
    void Render(Camera* camera);
    void Resize(int w, int h);

    // +++ Settings +++ -------------------------------------------------------


    // +++ Debug +++ ----------------------------------------------------------
    void SetDebugRendering(bool debugRenderingOn) { mDebugRendering = debugRenderingOn; }

private:

    // Renderers.
    DebugRenderer* mDebugRenderer;  // For debugging (axis, grid).
    PhongRenderer* mPhongRenderer;  // For most of textured objects (normal map too).

    // Debug data.
    bool mDebugRendering;
    AxisMesh* mOriginAxis;

    // XXX.
    TexturedQuadMesh* mTexturedQuad;
    QOpenGLTexture* colorMap;
    QOpenGLTexture* normalMap;
};

}  // namespace engine.