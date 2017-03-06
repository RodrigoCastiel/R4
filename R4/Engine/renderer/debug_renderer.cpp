#include "debug_renderer.h"

namespace engine
{

DebugRenderer::~DebugRenderer()
{
    delete mDebugShader;
}

void DebugRenderer::Bind(int renderingPass) 
{ 
    if (mDebugShader)
    {
    mDebugShader->bind();
    }
}

bool DebugRenderer::Load()
{
    // Allocate shader program.
    mDebugShader = new QOpenGLShaderProgram();

    // Load and build it.
    bool loadedVertexShader =
        mDebugShader->addShaderFromSourceFile(QOpenGLShader::Vertex, mVertexShaderPath);
    bool loadedFragmentShader =
        mDebugShader->addShaderFromSourceFile(QOpenGLShader::Fragment, mFragmentShaderPath);

    if (!loadedVertexShader || !loadedFragmentShader) 
    {
        qDebug() << "ERROR Could not load shaders (engine::DebugRenderer).\n";
        return false;
    }

    if (!mDebugShader->link())
    {
        qDebug() << "ERROR Could not link shaders (engine::DebugRenderer).\n";
        return false;
    }
  
    // Load uniform/attribute locations.
    mPositionAttribLoc = mDebugShader->attributeLocation("v_position");
    mColorAttribLoc    = mDebugShader->attributeLocation("v_color");
    mModelViewProjMatrixLoc = mDebugShader->uniformLocation("MVP");

    return true;
}

GLint DebugRenderer::GetAttribLocation(const QString& name, int renderingPass) const
{
    if (name == "position" || name == "v_position")
    {
    return mPositionAttribLoc;
    }
    else if (name == "color" || name == "v_color")
    {
    return mColorAttribLoc;
    }
    else  // There is no other attributes.
    {
    return -1;
    }
}

GLint DebugRenderer::GetUniformLocation(const QString & name, int renderingPass) const
{
    if (name == "MVP" || name == "model-view-projection" || name == "mvp")
    {
    return mModelViewProjMatrixLoc;
    }
    else  // There is no other uniforms.
    {
    return -1;
    }
}

void DebugRenderer::Render(const MeshGroup* mesh, const QMatrix4x4 & mvp, int pass) const
{
    mDebugShader->setUniformValue(mModelViewProjMatrixLoc, mvp);
    mesh->Render(pass);
}

}  // namespace engine.