#pragma once

// +--------------------------------------------+
// | File: gl_core.h                            |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-04.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

// This file has the global static variable that contains all OpenGL 4.3 functions.
// Please set the core value inside your QOpenGL widget class.
// In order to use any OpenGL function, use glCore->function() . 

#include <QOpenGLFunctions_4_3_Core>

namespace engine
{
    extern QOpenGLFunctions_4_3_Core* glCore;

}  // namespace engine.
