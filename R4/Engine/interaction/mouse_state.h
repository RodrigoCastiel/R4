// +--------------------------------------------+
// | File: mouse_state.h                        |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-05.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <QPoint>

namespace engine
{

struct MouseState
{
    // Cursor speed and position.
    QPoint mLastPos {0, 0};
    QPoint mSpeed   {0, 0};

    // Mouse buttons state.
    bool mLeftDown   { false };
    bool mRightDown  { false };
    bool mMiddleDown { false };
    
    // Keyboard command keys.
    bool mCtrlDown  { false };
    bool mShiftDown { false };
};

}  // namespace engine.