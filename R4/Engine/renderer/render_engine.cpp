#include "render_engine.h"

#include <QImage>

namespace engine
{

RenderEngine::RenderEngine()
{
    mDebugRendering = true;

    mDebugRenderer = new DebugRenderer();
    mPhongRenderer = new PhongRenderer("../shaders/normal_mapping_phong/vertex_shader.glsl", 
                                       "../shaders/normal_mapping_phong/fragment_shader.glsl");

    //mPhongRenderer = new PhongRenderer("../shaders/phong/vertex_shader.glsl", 
                                       //"../shaders/phong/fragment_shader.glsl");

    mOriginAxis = nullptr;
}

RenderEngine::~RenderEngine()
{
    delete mDebugRenderer;
    delete mPhongRenderer;

    delete mOriginAxis;

    // XXX.
    delete colorMap;
    delete normalMap;
}

bool RenderEngine::Load()
{
    glCore->glClearColor(0.1, 0.1, 0.1, 1.0);
    glCore->glEnable(GL_DEPTH_TEST);

    bool debugRendererLoaded = mDebugRenderer->Load();
    bool phongRendererLoaded = mPhongRenderer->Load();

    // Check if shaders were sucessfully loaded.
    if (!debugRendererLoaded || !phongRendererLoaded)
        return false;

    GLint phong_posAttr = mPhongRenderer->GetPositionAttribLoc();
    GLint phong_norAttr = mPhongRenderer->GetNormalAttribLoc();
    GLint phong_texAttr = mPhongRenderer->GetTextureAttribLoc();
    GLint phong_tanAttr = mPhongRenderer->GetTangentAttribLoc();

    mPhongRenderer->SetTextureUnit("color_map", 0);
    mPhongRenderer->SetTextureUnit("normal_map", 1);

    // Debug elements ---
    GLint debug_posAttr = mDebugRenderer->GetPositionAttribLoc();
    GLint debug_colAttr = mDebugRenderer->GetColorAttribLoc();

    mOriginAxis = new AxisMesh(debug_posAttr, debug_colAttr);

    // XXX.
    mTexturedQuad = new TexturedQuadMesh(phong_posAttr, phong_norAttr, 
                                         phong_texAttr, phong_tanAttr);

    // XXX.
    colorMap = new QOpenGLTexture(QImage("../textures/tiles/tile_color.jpg").mirrored());
    colorMap->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    colorMap->setMagnificationFilter(QOpenGLTexture::Linear);

    normalMap = new QOpenGLTexture(QImage("../textures/tiles/tile_normal.jpg").mirrored());
    normalMap->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    normalMap->setMagnificationFilter(QOpenGLTexture::Linear);

    return true;
}

void RenderEngine::Render(Camera* camera)
{
    glCore->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render Debug Meshes.
    if (mDebugRendering) 
    {
        mDebugRenderer->Bind();
        mDebugRenderer->Render(mOriginAxis->GetMeshGroup(), camera->GetProjView());
    }

    // (TODO) Render onto depth buffer all objects that generate shadow.

    // Render all objects.
    mPhongRenderer->Bind();
    mPhongRenderer->EnableLighting();
    mPhongRenderer->SetCamera(camera);

    // XXX.
    mPhongRenderer->SetNumLightSources(1);
    mPhongRenderer->EnableLightSource(0);
    mPhongRenderer->SetLightSourceCameraCoordinates({ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, 
                                                      {0.8f, 0.8f, 0.8f}, {0.2f, 0.2f, 0.2f}, 1.0f}, 0);

    mPhongRenderer->SetMaterial(mTexturedQuad->GetMaterial());
    mPhongRenderer->DisableColorMap();
    //mPhongRenderer->EnableColorMap();
    colorMap->bind(0);
    normalMap->bind(1);

    QMatrix4x4 model;
    model.setToIdentity();
    mPhongRenderer->SetModelMatrix(model);
    mTexturedQuad->Render();
    mPhongRenderer->EnableColorMap();

    // Render transparent objects such as leaves and trees.
}

void RenderEngine::Resize(int w, int h)
{
    glCore->glViewport(0, 0, w, h);
}

}  // namespace engine.