#pragma once

// This file has the global static variable that contains all OpenGL 4.3 functions.
// Please set the core value inside your QOpenGL widget class.
// In order to use any OpenGL function, use glCore->... .
//
// Author: Rodrigo Castiel, 2017.

#include <QOpenGLFunctions_4_3_Core>

namespace engine
{
    extern QOpenGLFunctions_4_3_Core* glCore;

}  // namespace engine.
