#include "phong_renderer.h"


namespace engine
{

PhongRenderer::~PhongRenderer()
{
    delete mPhongShader;
}

bool PhongRenderer::Load()
{
    // Allocate shader.
    mPhongShader = new QOpenGLShaderProgram();

    // Load and build it.
    bool loadedVertexShader =
        mPhongShader->addShaderFromSourceFile(QOpenGLShader::Vertex, mVertexShaderPath);
    bool loadedFragmentShader =
        mPhongShader->addShaderFromSourceFile(QOpenGLShader::Fragment, mFragmentShaderPath);

    if (!loadedVertexShader || !loadedFragmentShader) 
    {
        qDebug() << "ERROR Could not load shaders (engine::PhongRenderer).\n";
        return false;
    }

    if (!mPhongShader->link())
    {
        qDebug() << "ERROR Could not link shaders (engine::PhongRenderer).\n";
        return false;
    }

    mPhongShader->bind();

    // Get uniform/attribute locations.
    mPositionAttribLoc = mPhongShader->attributeLocation("v_position");
    mNormalAttribLoc   = mPhongShader->attributeLocation("v_normal");
    mTextureAttribLoc  = mPhongShader->attributeLocation("v_uv");
    mTangentAttribLoc  = mPhongShader->attributeLocation("v_tangent");

    mProjMatrixLoc   = mPhongShader->uniformLocation("P");
    mViewMatrixLoc   = mPhongShader->uniformLocation("V");
    mModelMatrixLoc  = mPhongShader->uniformLocation("M");
    mNormalMatrixLoc = mPhongShader->uniformLocation("N");

    // Pre-load material uniform pack.
    mMaterialUniform.mKaLoc = mPhongShader->uniformLocation("material.Ka");
    mMaterialUniform.mKdLoc = mPhongShader->uniformLocation("material.Kd");
    mMaterialUniform.mKsLoc = mPhongShader->uniformLocation("material.Ks");
    mMaterialUniform.mShininessLoc = mPhongShader->uniformLocation("material.alpha");
    mMaterialUniform.mOpacityLoc   = mPhongShader->uniformLocation("material.opacity");
    mMaterialUniform.mIllumLoc = mPhongShader->uniformLocation("material.illum");

    mUseColorMapUniform  = mPhongShader->uniformLocation("use_color_map");
    mUseNormalMapUniform = mPhongShader->uniformLocation("use_normal_map");

    // Pre-load light uniform packs.
    mLightingLoc = mPhongShader->uniformLocation("lighting");
    mNumLightUniform = mPhongShader->uniformLocation("num_lights");
    mLaLoc = mPhongShader->uniformLocation("La");

    for (int i = 0; i < kMaxNumberLights; i++)
    {
        QString light_prefix = "light[" + QString::number(i) + "].";

        mLightUniformArray[i].mPosLoc   = mPhongShader->uniformLocation(light_prefix + "pos");
        mLightUniformArray[i].mDirLoc   = mPhongShader->uniformLocation(light_prefix + "dir");
        mLightUniformArray[i].mLdLoc    = mPhongShader->uniformLocation(light_prefix + "Ld");
        mLightUniformArray[i].mLsLoc    = mPhongShader->uniformLocation(light_prefix + "Ls");
        mLightUniformArray[i].mIsSpotlightLoc 
            = mPhongShader->uniformLocation(light_prefix + "is_spotlight");
        mLightUniformArray[i].mSpotlightAngleLoc 
            = mPhongShader->uniformLocation(light_prefix + "cone_angle");

        mLightSwitchUniformArray[i] = mPhongShader->uniformLocation("light_switch[" + QString::number(i) + "]");
    }

    return true;
}

void PhongRenderer::Bind(int renderingPass)
{
    if (mPhongShader) 
    {
        mPhongShader->bind();
    }
}

void PhongRenderer::Render(const MeshGroup* mesh, const QMatrix4x4 & model, Camera* camera, int pass) const
{
    PhongRenderer::SetCamera(camera);
    PhongRenderer::SetModelMatrix(model);
    mesh->Render(pass);
}

void PhongRenderer::Render(const MeshGroup* mesh, const QMatrix4x4 & model, int pass) const
{
    PhongRenderer::SetModelMatrix(model);
    mesh->Render(pass);
}

GLint PhongRenderer::GetAttribLocation(const QString & name, int renderingPass) const
{
  if (name == "position" || name == "v_position")
  {
    return mPositionAttribLoc;
  }
  else if (name == "texture" || name == "v_uv" || name == "uv")
  {
    return mTextureAttribLoc;
  }
  else if (name == "normal" || name == "v_normal")
  {
    return mNormalAttribLoc;
  }
  else if (name == "tangent" || name == "v_tangent")
  {
    return mTangentAttribLoc;
  }
  else  // Search it up.
  {
    return mPhongShader->attributeLocation(name);
  }
}

GLint PhongRenderer::GetUniformLocation(const QString & name, int renderingPass) const
{
  if (name == "M" || name == "model")
  {
    return mModelMatrixLoc;
  }
  else if (name == "V" || name == "view")
  {
    return mViewMatrixLoc;
  }
  else if (name == "P" || name == "projection" || name == "proj")
  {
    return mProjMatrixLoc;
  }
  else if (name == "N" || name == "normal")
  {
    return mNormalMatrixLoc;
  }
  else // Search it up.
  {
    return mPhongShader->uniformLocation(name);
  }
}

void PhongRenderer::SetLightAmbientComponent(const QVector3D & La) 
{
    mPhongShader->setUniformValue(mLaLoc, La);
}

void PhongRenderer::SetLightSourceCameraCoordinates(const LightSource & lightSource, int slot) 
{
    const LightUniformPack & lightSourceUniform = mLightUniformArray[slot];

    mPhongShader->setUniformValue(lightSourceUniform.mPosLoc, lightSource.mPos);  // Position.
    mPhongShader->setUniformValue(lightSourceUniform.mLdLoc,  lightSource.mLd);  // Diffuse component.
    mPhongShader->setUniformValue(lightSourceUniform.mLsLoc,  lightSource.mLs);  // Specular component.
    mPhongShader->setUniformValue(lightSourceUniform.mIsSpotlightLoc, (int)lightSource.mIsSpotlight);

    // Spotlight attributes:
    if (lightSource.mIsSpotlight)
    {
        mPhongShader->setUniformValue(lightSourceUniform.mDirLoc, lightSource.mDir);  // Direction.
        mPhongShader->setUniformValue(lightSourceUniform.mIsSpotlightLoc, 1);
        mPhongShader->setUniformValue(  // Cone angle.
            lightSourceUniform.mSpotlightAngleLoc, lightSource.mSpotlightAngle/2.0f);  
    }
    else
    {
        mPhongShader->setUniformValue(lightSourceUniform.mIsSpotlightLoc, 0);
    }
}

void PhongRenderer::SetLightSourceWorldCoordinates(const LightSource & lightSource, 
                                                      const Camera * camera, int slot) 
{
    const LightUniformPack & lightSourceUniform = mLightUniformArray[slot];

    // Transform position/direction into camera coordinates.
    QVector4D p = QVector4D(lightSource.mPos, 1.0f);
    QVector4D d = QVector4D(lightSource.mDir, 0.0f);
    const QMatrix4x4 & V = camera->GetViewMatrix();

    p = V * p;
    d = V * d;
    p = p / p[3];  // Normalize homogenous coordinates.

    mPhongShader->setUniformValue(lightSourceUniform.mPosLoc, p.toVector3D());   // Position.
    mPhongShader->setUniformValue(lightSourceUniform.mLdLoc,  lightSource.mLd);  // Diffuse component.
    mPhongShader->setUniformValue(lightSourceUniform.mLsLoc,  lightSource.mLs);  // Specular component.
    mPhongShader->setUniformValue(lightSourceUniform.mIsSpotlightLoc, (int)lightSource.mIsSpotlight);

    // Spotlight attributes:
    if (lightSource.mIsSpotlight)
    {
        mPhongShader->setUniformValue(lightSourceUniform.mDirLoc, lightSource.mDir);  // Direction.
        mPhongShader->setUniformValue(lightSourceUniform.mIsSpotlightLoc, 1);
        mPhongShader->setUniformValue(  // Cone angle.
            lightSourceUniform.mSpotlightAngleLoc, lightSource.mSpotlightAngle/2.0f);  
    }
    else
    {
        mPhongShader->setUniformValue(lightSourceUniform.mIsSpotlightLoc, 0);
    }
}

void PhongRenderer::SetMaterial(const Material & material) 
{
    mPhongShader->setUniformValue(mMaterialUniform.mKaLoc, material.mKa);  // Ambient component.
    mPhongShader->setUniformValue(mMaterialUniform.mKdLoc, material.mKd);  // Diffuse component.
    mPhongShader->setUniformValue(mMaterialUniform.mKsLoc, material.mKs);  // Specular component.

    mPhongShader->setUniformValue(mMaterialUniform.mIllumLoc,     material.mIllum);      // Illumination type.
    mPhongShader->setUniformValue(mMaterialUniform.mOpacityLoc,   material.mOpacity);    // Opacity.
    mPhongShader->setUniformValue(mMaterialUniform.mShininessLoc, material.mShininess);  // Alpha.

    if (material.mDiffuseMap)  // If it has a valid diffuse map.
    {
        PhongRenderer::EnableColorMap();
        material.mDiffuseMap->bind(0);
    }
    else
    {
        PhongRenderer::DisableColorMap();
    }
}

}  // namespace engine.