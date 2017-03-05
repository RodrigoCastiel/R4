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
#include "interaction/mouse_state.h"

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

};

}  // namespace engine.