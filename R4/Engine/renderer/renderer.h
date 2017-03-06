// +--------------------------------------------+
// | File: renderer.h                           |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-05.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <vector>
#include <string>

#include <QOpenGLShaderProgram>

namespace engine
{

class Renderer
{
public:
  // Constructor.
  Renderer()  { }

  // Destructor.
  virtual ~Renderer() { }

  // Binds the corresponding shader program no. 'renderingPass'.
  virtual void Bind(int renderingPass = 0) = 0;

  // Does setup of attributes.
  virtual bool Load() = 0;

  // Renders a scene containing a list of light sources and objects through the point of view
  // of camera.
  virtual void Render() const { }

  // Pure virtual getter and setter methods.
  virtual unsigned GetNumRenderingPasses() const = 0;
  virtual const QOpenGLShaderProgram* GetShaderProgram(int renderingPass = 0) const = 0;

  // Get attribute/uniform for the corresponding rendering pass.
  virtual GLint GetAttribLocation( const QString & name, int renderingPass = 0) const = 0;
  virtual GLint GetUniformLocation(const QString & name, int renderingPass = 0) const = 0;
};

}  // namespace gloo.




