#include "game_engine.h"

namespace engine
{

GameEngine::GameEngine()
    : mRenderEngine(nullptr)
{
    mRenderEngine = new RenderEngine();

    // XXX.
    debugRenderer = new DebugRenderer();
    camera = new TrackballCamera();
    camera->SetDistanceToTarget(4.0f);

}

GameEngine::~GameEngine()
{
    delete mRenderEngine;
}

// ================================================================================================

void GameEngine::Load()
{
    mRenderEngine->Load();

    // XXX.
    bool loaded = debugRenderer->Load();

    GLint posAttribLoc = debugRenderer->GetPositionAttribLoc();
    GLint colAttribLoc = debugRenderer->GetColorAttribLoc();

    axis = new AxisMesh(posAttribLoc, colAttribLoc);

}

void GameEngine::Render()
{
    mRenderEngine->Render();


    // XXX.
    //camera->SetPhi(20.0f);
    //camera->RotateTheta(0.1f);
    //camera->RotatePhi(0.04f);

    QVector3D target = {1.0f, 10.0f, 2.0f};

    camera->SetTarget(target);
    camera->UpdateViewMatrix();
    debugRenderer->Bind();
    debugRenderer->Render(axis->GetMeshGroup(), camera->GetProjView());

    QMatrix4x4 model;
    model.setToIdentity();
    model.translate(target);
    debugRenderer->Render(axis->GetMeshGroup(), camera->GetProjView());
    debugRenderer->Render(axis->GetMeshGroup(), camera->GetProjView()*model);
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