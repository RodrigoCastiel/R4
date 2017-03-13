// +--------------------------------------------+
// | File: camera.h                             |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-06.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

// General-purpose phong renderer for entities.

#pragma once 

#include <QOpenGLShaderProgram>

#include "core/gl_core.h"

#include "camera/camera.h"
#include "entities/light.h"

#include "mesh/mesh_group.h"
#include "mesh/material_lib.h"

namespace engine 
{

const int kMaxNumberLights = 8;

class PhongRenderer
{
public:
  // Load a custom phong renderer if you need it. 
  PhongRenderer(const QString & vertexShaderPath,
                const QString & fragmentShaderPath)
  : mVertexShaderPath(vertexShaderPath)
  , mFragmentShaderPath(fragmentShaderPath)
  { }

  PhongRenderer()
  : mVertexShaderPath(  "../shaders/phong/vertex_shader.glsl")
  , mFragmentShaderPath("../shaders/phong/fragment_shader.glsl")
  { }

  ~PhongRenderer();

  // Load() initializes all uniform/attribute locations for fast access.
  // Please call it after allocating a new PhongRenderer instance.
  bool Load();

  void Render(const MeshGroup* mesh, const QMatrix4x4 & model, Camera* camera, int pass=0) const;

  void Render(const MeshGroup* mesh, const QMatrix4x4 & model, int pass=0) const;

  // Call bind before using PhongRenderer. Internally, it calls glUseProgram().
  void Bind(int renderingPass = 0);

  void SetCamera(const Camera* camera) const;
  void SetModelMatrix(const QMatrix4x4 & model) const;

  inline unsigned GetNumRenderingPasses() const { return 1; }
  inline const QOpenGLShaderProgram* GetShaderProgram(int renderingPass = 0) const { return mPhongShader; }

  GLint GetAttribLocation( const QString & name, int renderingPass = 0) const;
  GLint GetUniformLocation(const QString & name, int renderingPass = 0) const;

  // Extra methods (fast access).
  inline GLint GetPositionAttribLoc() const { return mPositionAttribLoc; }
  inline GLint GetTextureAttribLoc()  const { return mTextureAttribLoc;  }
  inline GLint GetNormalAttribLoc()   const { return mNormalAttribLoc;   }
  inline GLint GetTangentAttribLoc()  const { return mTangentAttribLoc;  }

  inline GLint GetViewUniformLoc()   const { return mViewMatrixLoc; }
  inline GLint GetProjUniformLoc()   const { return mProjMatrixLoc; }
  inline GLint GetModelUniformLoc()  const { return mModelMatrixLoc;  }
  inline GLint GetNormalUniformLoc() const { return mNormalMatrixLoc; }

  GLint GetLightAmbientComponentLoc() const { return mLaLoc; }
  LightUniformPack GetLightSourceUniformLoc(int slot) const { return mLightUniformArray[slot]; }
  MaterialUniformPack GetMaterialUniformLoc() const { return mMaterialUniform; }

  // === Lighting configuration methods ===
  
  void EnableLighting() const;
  void DisableLighting() const;

  // Activate light source on shader.
  void EnableLightSource(int slot)  const;
  // Deactivate light source on shader.
  void DisableLightSource(int slot) const;

  // Change number of light sources on shader (for rendering optimization).
  void SetNumLightSources(int numLightSources) const;

  // Set light source/light properties.
  void SetLightAmbientComponent(const QVector3D & La) const;
 
  // Sets light source at shader 'slot'. 
  // Light source must be in camera coordinates. 
  // Use this if you want the source to follow camera.
  void SetLightSourceCameraCoordinates(const LightSource & lightSource, int slot) const;

  // Sets light source at shader 'slot'. 
  // Light source must be in world coordinates.
  void SetLightSourceWorldCoordinates(
      const LightSource & lightSource, const Camera * camera, int slot) const;

  // === Material configuration methods ===
  void SetMaterial(const Material & material) const;

  // === Texture configuration methods ===

  void EnableColorMap();
  void DisableColorMap();

  void EnableNormalMap();
  void DisableNormalMap();

  // Use the following methods to link a texture unit to a sampler on shader.
  // samplerName is your sampler uniform name on the shader.
  // slot is the number of the texture unit.
  void SetTextureUnit(const char * samplerName, GLuint slot) const;
  void SetTextureUnit(const QString & samplerName, GLuint slot) const;

private:
  // Shader Program.
  QOpenGLShaderProgram* mPhongShader { nullptr };

  // Fast-access attribute/uniform locations.
  GLint mPositionAttribLoc { -1 };
  GLint mTextureAttribLoc  { -1 };
  GLint mNormalAttribLoc   { -1 };
  GLint mTangentAttribLoc  { -1 };

  GLint mViewMatrixLoc   { -1 };
  GLint mProjMatrixLoc   { -1 };
  GLint mModelMatrixLoc  { -1 };
  GLint mNormalMatrixLoc { -1 };

  // Lighting.
  GLint mLightingLoc { -1 };
  GLint mLaLoc { -1 };
  GLint mNumLightUniform { -1 };
  GLint mLightSwitchUniformArray[kMaxNumberLights];
  LightUniformPack mLightUniformArray[kMaxNumberLights];

  // Material and Texture.
  GLint mUseColorMapUniform  { -1 };
  GLint mUseNormalMapUniform { -1 };
  MaterialUniformPack mMaterialUniform;  // Set of material uniforms.

  // Constant data (passed to constructor).
  const QString mVertexShaderPath;
  const QString mFragmentShaderPath;
};

// ----- Inline methods ---------------------------------------------------------------------------

inline
void PhongRenderer::SetTextureUnit(const char * samplerName, GLuint slot) const
{
  mPhongShader->setUniformValue(samplerName, slot);
}

inline
void PhongRenderer::SetTextureUnit(const QString & samplerName, GLuint slot) const
{
  PhongRenderer::SetTextureUnit(samplerName.toStdString().c_str(), slot);
}

inline
void PhongRenderer::SetCamera(const Camera* camera) const
{
    mPhongShader->setUniformValue(mProjMatrixLoc, camera->GetProjMatrix());
    mPhongShader->setUniformValue(mViewMatrixLoc, camera->GetViewMatrix());
}

inline
void PhongRenderer::SetModelMatrix(const QMatrix4x4 & model) const
{
    // Set Model matrix and Normal matrix.
    mPhongShader->setUniformValue(mModelMatrixLoc, model);
    mPhongShader->setUniformValue(mNormalMatrixLoc, model.inverted().transposed());
}

inline
void PhongRenderer::SetNumLightSources(int numLightSources) const
{
    // Make sure that (0 <= numLightSources <= kMaxNumberLights).
    numLightSources = std::max(0, std::min(kMaxNumberLights, numLightSources));
    mPhongShader->setUniformValue(mNumLightUniform, numLightSources);
}

inline
void PhongRenderer::EnableLightSource(int slot)  const
{
    mPhongShader->setUniformValue(mLightSwitchUniformArray[slot], 1);
}

inline
void PhongRenderer::DisableLightSource(int slot) const
{
    mPhongShader->setUniformValue(mLightSwitchUniformArray[slot], 0);
}

inline
void PhongRenderer::EnableLighting()  const
{
    mPhongShader->setUniformValue(mLightingLoc, 1);
}

inline
void PhongRenderer::DisableLighting() const
{
    mPhongShader->setUniformValue(mLightingLoc, 0);
}

inline
void PhongRenderer::EnableColorMap()
{
    mPhongShader->setUniformValue(mUseColorMapUniform, 1);
}

inline
void PhongRenderer::DisableColorMap()
{
    mPhongShader->setUniformValue(mUseColorMapUniform, 0);
}

inline
void PhongRenderer::EnableNormalMap()
{
    mPhongShader->setUniformValue(mUseNormalMapUniform, 1);
}

inline
void PhongRenderer::DisableNormalMap()
{
    mPhongShader->setUniformValue(mUseNormalMapUniform, 0);
}

}  // namespace gloo.
