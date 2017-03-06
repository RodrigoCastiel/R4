// +--------------------------------------------+
// | File: game_engine.h                        |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-04.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <QKeyEvent>
#include <QMouseEvent>

#include "helper/useful_meshes.h"
#include "renderer/render_engine.h"
#include "interaction/mouse_state.h"

// XXX.
#include "renderer/debug_renderer.h"
#include "camera/trackball_camera.h"

namespace engine
{

class GameEngine
{
public:
    // +++ Constructors/Destructor +++ ----------------------------------------
    GameEngine();
    ~GameEngine();

    // +++ Main +++ -----------------------------------------------------------

    void Load();
    void Update();
    void Render();
    void Resize(int w, int h);

    // ++ User-Interface +++ --------------------------------------------------
    
    void OnKeyboardPress(const QSet<int> & pressedKeys);
    void OnKeyboardRelease(const QSet<int> & pressedKeys);

    void OnMouseMove(const MouseState & mouseState);

    void OnMouseLeftDown(const MouseState & mouseState);
    void OnMouseLeftUp(const MouseState & mouseState);

    void OnMouseRightDown(const MouseState & mouseState);
    void OnMouseRightUp(const MouseState & mouseState);

    void OnMouseMiddleDown(const MouseState & mouseState);
    void OnMouseMiddleUp(const MouseState & mouseState);

private:
    RenderEngine* mRenderEngine;

    // XXX.
    TrackballCamera* camera;
    DebugRenderer* debugRenderer;
    AxisMesh* axis;
};

}  // namespace engine.