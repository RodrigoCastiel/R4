#include "render_engine.h"

#include <QImage>
#include "mesh/material_lib.h"
#include "helper/r4o_file.h"
#include "mesh/static_object.h"

namespace engine
{

RenderEngine::RenderEngine()
{
    mDebugRendering = true;

    mDebugRenderer = new DebugRenderer();
    mPhongRenderer = new PhongRenderer("../shaders/normal_mapping_phong/vertex_shader.glsl", 
                                       "../shaders/normal_mapping_phong/fragment_shader.glsl");

    mOriginAxis = nullptr;
}

RenderEngine::~RenderEngine()
{
    delete mDebugRenderer;
    delete mPhongRenderer;

    delete mOriginAxis;

    // XXX.
    //delete terrainChunk;
    //delete testMesh;
    delete sceneObj;
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
    terrainChunk = new TerrainChunk(3.0f, 350.0f);
    terrainChunk->Load("../scenery/heightmap/yosemite4.png", "");

    sceneObj = new StaticObject();
    //sceneObj->LoadFromR4O("../models/simple-trees/BlenderNatureAsset.r4o");
    //sceneObj->LoadFromR4O("../models/peugeot-207/Peugeot_207.r4o");
    //sceneObj->LoadFromR4O("../models/grass/Grass_02.r4o");
    //sceneObj->LoadFromR4O("../models/The City/city2.r4o");
    sceneObj->LoadFromR4O("../models/raider/raider.r4o");
    //sceneObj->LoadFromR4O("../models/hand-flashlight/hand_light.r4o");
        //sceneObj->LoadFromR4O("../models/x-wing/star wars x-wing.r4o");

    return true;
}

void RenderEngine::Render(Camera* camera)
{
    glCore->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glCore->glEnable(GL_CULL_FACE);
    glCore->glCullFace(GL_FRONT);
    glCore->glFrontFace(GL_CW);

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
    mPhongRenderer->SetNumLightSources(2);
    mPhongRenderer->EnableLightSource(0);
    mPhongRenderer->EnableLightSource(1);
    mPhongRenderer->SetLightSourceCameraCoordinates({ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, 
                                                      {0.4f, 0.4f, 0.4f}, {0.1f, 0.1f, 0.1f}, 0, 90.0f}, 0);

    mPhongRenderer->SetLightSourceWorldCoordinates({ {500.0f, 1000.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, 
                                                     {0.8f, 0.8f, 0.8f}, {0.0f, 0.0f, 0.0f}, 0, 45.0f }, camera, 1);

    mPhongRenderer->EnableColorMap();
    mPhongRenderer->EnableNormalMap();

    static float theta = 0.0;
    theta += 0.01;

    QMatrix4x4 model;
    model.setToIdentity();
    model.rotate(theta*4.0f, 0.0, 0.0, 1.0);
    //model.translate(3.0f*std::cos(theta), 0.0, 0.0);

    mPhongRenderer->SetModelMatrix(model);

    mPhongRenderer->DisableColorMap();
    mPhongRenderer->DisableNormalMap();
    model.setToIdentity();


    mPhongRenderer->SetModelMatrix(model);
    mPhongRenderer->EnableColorMap();
    terrainChunk->Render(mPhongRenderer);

    model.scale(1e-2);
    mPhongRenderer->SetModelMatrix(model);
    sceneObj->Render(mPhongRenderer);
    
    // Render transparent objects such as leaves and trees.
}

void RenderEngine::Resize(int w, int h)
{
    glCore->glViewport(0, 0, w, h);
}

}  // namespace engine.