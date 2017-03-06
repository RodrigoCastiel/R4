// +--------------------------------------------+
// | File: debug_renderer.h                     |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-05.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include "renderer.h"

#include <QString>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

#include "mesh/mesh_group.h"

namespace engine
{

class DebugRenderer : public Renderer
{
public:
  DebugRenderer(const QString & vertexShaderPath,
                const QString & fragmentShaderPath)
  : Renderer()
  , mVertexShaderPath(vertexShaderPath)
  , mFragmentShaderPath(fragmentShaderPath)
  { }

  // Default constructor.
  DebugRenderer()
  : Renderer()
  , mVertexShaderPath(  "../shaders/debug/vertex_shader.glsl")
  , mFragmentShaderPath("../shaders/debug/fragment_shader.glsl")
  { }

  ~DebugRenderer();

  bool Load();

  virtual void Bind(int renderingPass = 0);

  // Renders a specific object through a point of view.
  void Render(const MeshGroup* mesh, const QMatrix4x4 & mvp, int pass=0) const;

  inline 
  unsigned GetNumRenderingPasses() const { return 1; }

  inline 
  const QOpenGLShaderProgram* GetShaderProgram(int renderingPass = 0) const { return mDebugShader; }
  
  GLint GetAttribLocation( const QString & name, int renderingPass = 0) const;
  GLint GetUniformLocation(const QString & name, int renderingPass = 0) const;

  // Extra methods.
  GLint GetPositionAttribLoc() const { return mPositionAttribLoc; }
  GLint GetColorAttribLoc()    const { return mColorAttribLoc;    }
  GLint GetModelViewProjUniformLoc() const { return mModelViewProjMatrixLoc; }

protected:
  // Shader program.
  QOpenGLShaderProgram* mDebugShader { nullptr };

  // Main attribute/uniform locations.
  // It works as fast access variables (without querying the GPU).
  GLint mPositionAttribLoc { -1 };
  GLint mColorAttribLoc    { -1 };
  GLint mModelViewProjMatrixLoc { -1 };

  // Constant data (passed to constructor).
  const QString mVertexShaderPath;
  const QString mFragmentShaderPath;
};

}  // namespace engine.
