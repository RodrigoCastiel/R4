#include "game_engine.h"

namespace engine
{

GameEngine::GameEngine()
    : mRenderEngine(nullptr)
{
    mRenderEngine = new RenderEngine();

    // XXX.
    camera = new TrackballCamera();
    camera->SetDistanceToTarget(4.0f);

}

GameEngine::~GameEngine()
{
    delete camera;

    delete mRenderEngine;
}

// ================================================================================================

void GameEngine::Load()
{
    mRenderEngine->Load();
}

void GameEngine::Render()
{
    camera->UpdateViewMatrix();

    mRenderEngine->Render(camera);
}

void GameEngine::Update()
{
    // Do game loop tasks.
    // 1. Collision detection.
    // 2. Position update.
    // 3. Add/Remove/Update entities.
    // 4. Update renderer accordingly.
}

void GameEngine::Resize(int w, int h)
{
    mRenderEngine->Resize(w, h);

    // XXX.
    camera->UpdateProjMatrix(0, 0, w, h);
}

// ================================================================================================

void GameEngine::OnKeyboardPress(const QSet<int> & pressedKeys)
{
    
}

void GameEngine::OnKeyboardRelease(const QSet<int> & pressedKeys)
{

}

// ================================================================================================

void GameEngine::OnMouseMove(const MouseState & mouseState)
{
    if (mouseState.mLeftDown)
    {

    }

    if (mouseState.mRightDown)
    {
        camera->Rotate(mouseState.mSpeed);
    }

    if (mouseState.mMiddleDown)
    {
        camera->Dolly(mouseState.mSpeed);
    }
}

void GameEngine::OnMouseLeftDown(const MouseState & mouseState)
{

}

void GameEngine::OnMouseLeftUp(const MouseState & mouseState)
{

}

void GameEngine::OnMouseRightDown(const MouseState & mouseState)
{

}

void GameEngine::OnMouseRightUp(const MouseState & mouseState)
{

}

void GameEngine::OnMouseMiddleDown(const MouseState & mouseState)
{

}

void GameEngine::OnMouseMiddleUp(const MouseState & mouseState)
{

}

// ================================================================================================


}  // namespace engine.