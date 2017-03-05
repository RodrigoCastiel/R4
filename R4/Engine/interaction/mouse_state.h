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
    QPoint mLastPos;
    QPoint mSpeed;
};

}  // namespace engine.