#include "game_engine.h"

namespace engine
{

GameEngine::GameEngine()
    : mRenderEngine(nullptr)
{
    mRenderEngine = new RenderEngine();
    mLockMouseCursor = true;

    // XXX.
    camera = new TrackballCamera();
    camera->SetDistanceToTarget(500.0f);

    fpvCamera = new FPVCamera();
    fpvCamera->RotatePitch(90.0f);
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
    fpvCamera->UpdateViewMatrix();

    //mRenderEngine->Render(camera);
    mRenderEngine->Render(fpvCamera);
}

void GameEngine::Update(const MouseState & mouseState, const QSet<int> & pressedKeys)
{
    // Move camera according to mouse.
    fpvCamera->MoveWASD(pressedKeys.contains(Qt::Key_W), pressedKeys.contains(Qt::Key_A),
                        pressedKeys.contains(Qt::Key_S), pressedKeys.contains(Qt::Key_D), 0.1f);
    // Correct its positions according to the terrain.
    QVector3D center = fpvCamera->GetCenter();
    Terrain* terrain = mRenderEngine->GetTerrain();
    center.setY(terrain->Geometry().HeightAt(center.x(), center.z()) + 1.8f);
    fpvCamera->SetCenter(center);

    //fpvCamera->RotateYaw(0.05);
    //fpvCamera->MoveForward(0.1f);

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
    fpvCamera->UpdateProjMatrix(0, 0, w, h);
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
    fpvCamera->Rotate(mouseState.mSpeed);

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