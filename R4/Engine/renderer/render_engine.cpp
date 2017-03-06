#include "render_engine.h"

namespace engine
{

RenderEngine::RenderEngine()
{

}

RenderEngine::~RenderEngine()
{

}

void RenderEngine::Load()
{
    glCore->glEnable(GL_DEPTH_TEST);
}

void RenderEngine::Render()
{

}

void RenderEngine::Resize(int w, int h)
{
    glCore->glViewport(0, 0, w, h);
}

}  // namespace engine.