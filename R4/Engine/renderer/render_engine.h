// +--------------------------------------------+
// | File: render_engine.h                      |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-05.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include "core/gl_core.h"

namespace engine
{

class RenderEngine
{
public:
    // +++ Constructor/Destructor +++ -----------------------------------------
    RenderEngine();
    ~RenderEngine();

    // +++ Main +++ -----------------------------------------------------------

    void Load();
    void Render();
    void Resize(int w, int h);

    // +++ Settings +++ -------------------------------------------------------

private:

};

}  // namespace engine.