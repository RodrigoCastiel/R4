#include "useful_meshes.h"

namespace engine
{

// ============================================================================================= //

AxisMesh::AxisMesh(GLint positionAttribLoc, GLint colorAttribLoc)
{
  const int numVertices = 6;
  const int numElements = 6;
  const GLenum drawMode = GL_LINES;

  // Initialize vertices.
  GLfloat positions[] = {0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
                         0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f};
  GLfloat colors [] =   {1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
                         0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
                         0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f};
  const GLuint* indices = nullptr;

  // Allocate mesh.
  mMeshGroup = new MeshGroup(numVertices, numElements, drawMode);

  // Specify its attributes.
  mMeshGroup->SetVertexAttribList({3, 3});

  // Add rendering pass.
  mMeshGroup->AddRenderingPass({{positionAttribLoc, true}, {colorAttribLoc, true}});

  // Load data.
  mMeshGroup->Load({positions, colors}, indices);
}

AxisMesh::~AxisMesh()
{
  delete mMeshGroup;
}

void AxisMesh::Update(GLfloat x, GLfloat y, GLfloat z)
{
  GLfloat positions[] = {0.0f, 0.0f, 0.0f,     x, 0.0f, 0.0f,
                         0.0f, 0.0f, 0.0f,  0.0f,    y, 0.0f,
                         0.0f, 0.0f, 0.0f,  0.0f, 0.0f,    z};

  mMeshGroup->Update({positions, nullptr});
}

void AxisMesh::Render() const
{
  mMeshGroup->Render();
}

// ============================================================================================= //

BoundingBoxMesh::BoundingBoxMesh(GLint positionAttribLoc, GLint colorAttribLoc, 
                                 const QVector3D & rgb)
{
  const int numVertices = 8;
  const int numElements = 12*2;
  const GLenum drawMode = GL_LINES;

  const GLfloat r = rgb[0];
  const GLfloat g = rgb[1];
  const GLfloat b = rgb[2];

  // Initialize vertices.
  GLfloat positions[] = {-0.5f, +0.5f, +0.5f,  +0.5f, +0.5f, +0.5f,
                         -0.5f, +0.5f, -0.5f,  +0.5f, +0.5f, -0.5f,
                         -0.5f, -0.5f, +0.5f,  +0.5f, -0.5f, +0.5f,
                         -0.5f, -0.5f, -0.5f,  +0.5f, -0.5f, -0.5f };

  GLfloat colors [] =   {r, g, b, r, g, b,
                         r, g, b, r, g, b,
                         r, g, b, r, g, b,
                         r, g, b, r, g, b};
  GLuint indices[] = { 0,1, 2,3, 4,5, 6,7,    // x
                       0,2, 1,3, 4,6, 5,7,    // z
                       0,4, 1,5, 2,6, 3,7 };  // z

  // Allocate mesh.
  mMeshGroup = new MeshGroup(numVertices, numElements, drawMode);

  // Specify its attributes.
  mMeshGroup->SetVertexAttribList({3, 3});

  // Add rendering pass.
  mMeshGroup->AddRenderingPass({{positionAttribLoc, true}, {colorAttribLoc, true}});

  // Load data.
  mMeshGroup->Load({positions, colors}, indices);
}

BoundingBoxMesh::~BoundingBoxMesh()
{
  delete mMeshGroup;
}

void BoundingBoxMesh::Update(GLfloat xmin, GLfloat xmax, GLfloat ymin, GLfloat ymax, GLfloat zmin, GLfloat zmax)
{
  GLfloat positions[] = {xmin, ymax, zmax,  xmax, ymax, zmax,
                         xmin, ymax, zmin,  xmax, ymax, zmin,
                         xmin, ymin, zmax,  xmax, ymin, zmax,
                         xmin, ymin, zmin,  xmax, ymin, zmin };

  mMeshGroup->Update({positions, nullptr});
}

void BoundingBoxMesh::Render() const
{
  mMeshGroup->Render();
}

// ============================================================================================= //

GridMesh::GridMesh(GLint positionAttribLoc, GLint colorAttribLoc, int width, int height, 
                   GLfloat tileSize, const QVector3D & rgb)
{
  const int w = std::max(width,  2);
  const int h = std::max(height, 2);

  const int numVertices = w * h;
  const int numElements = 2 * w * h;
  const GLenum drawMode = GL_LINE_STRIP;

  std::vector<GLfloat> vertices;
  std::vector<GLuint> indices;

  const GLfloat r = rgb[0];
  const GLfloat g = rgb[1];
  const GLfloat b = rgb[2];

  vertices.reserve(numVertices * 6);

  float xc = (w-1)*tileSize/2.0f;
  float yc = 0.0f;
  float zc = (h-1)*tileSize/2.0f;

  // Initialize vertices.
  for (int z = 0; z < h; z++)
  {
    for (int x = 0; x < w; x++)
    {
      // Positions x, y, z.
      vertices.push_back(x*tileSize - xc);
      vertices.push_back(0.0f);
      vertices.push_back(z*tileSize - zc);

      // Colors.
      vertices.push_back(r);
      vertices.push_back(g);
      vertices.push_back(b);
    }
  }

  indices.reserve(numElements);

  // Wire frame Element array - indices are written in a zig-zag pattern,
  // first horizontally and then vertically. It uses GL_LINE_STRIP. 
  int index = 0;
  int x = 0, y = 0;
  int dx = 1, dy = -1;

  for (y = 0; y < h; y++)  // Horizontally.
  {
    x = ((dx == 1) ? 0 : w-1);
    for (int k = 0; k < w; k++, x += dx)
      indices.push_back(w*x + y);
    dx *= -1;
  }

  // Start from the last point to allow continuity in GL_LINE_STRIP.
  x = ((dx == 1) ? 0 : w-1);
  y = h-1;  
  dy = -1;
  for (int i = 0; i < w; i++, x += dx)  // Vertically.
  {
    y = ((dy == 1) ? 0 : h-1);
    for (int j = 0; j < h; j++, y += dy)
      indices.push_back(w*x + y);
    dy *= -1;
  }

  // Allocate mesh.
  mMeshGroup = new MeshGroup(numVertices, numElements, drawMode);

  // Specify its attributes.
  mMeshGroup->SetVertexAttribList({3, 3});

  // Add rendering pass.
  mMeshGroup->AddRenderingPass({{positionAttribLoc, true}, {colorAttribLoc, true}});

  // Load data.
  // mMeshGroup->Load({positions.data(), colors.data()}, indices.data());
  mMeshGroup->Load(vertices.data(), indices.data());
}

GridMesh::~GridMesh()
{
  delete mMeshGroup;
}

void GridMesh::Render() const
{
  mMeshGroup->Render();
}

// ============================================================================================= //

TexturedQuadMesh::TexturedQuadMesh(GLint posAttr, GLint norAttr, GLint uvAttr, GLint tangAttr)
{
    const int numVertices = 4;
    const int numElements = 6;
    const GLenum drawMode = GL_TRIANGLES;

    // Initialize vertices.
    GLfloat positions[] = {-1.0f, +1.0f, 0.0f,  +1.0f, +1.0f, 0.0f,
                           +1.0f, -1.0f, 0.0f,  -1.0f, -1.0f, 0.0f};
    GLfloat normals [] =   { 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
                             0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f};
    GLfloat uvs[] = { 0.0f, 1.0f,  1.0f, 1.0f,
                      1.0f, 0.0f,  0.0f, 0.0f};

    GLfloat tangents[] = {  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
                            1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f};

    const GLuint indices[] = {0, 1, 2, 2, 3, 0};

    // Allocate mesh.
    mMeshGroup = new MeshGroup(numVertices, numElements, drawMode);

    // Specify its attributes and add rendering pass. 
    if (tangAttr != -1) 
    {
        mMeshGroup->SetVertexAttribList({3, 3, 2, 3});
        mMeshGroup->AddRenderingPass({{posAttr, true}, 
                                      {norAttr, true}, 
                                      {uvAttr, true},
                                      {tangAttr, true}});
        mMeshGroup->Load({positions, normals, uvs, tangents}, indices);
    } 
    else 
    {
        mMeshGroup->SetVertexAttribList({3, 3, 2});
        mMeshGroup->AddRenderingPass({{posAttr, true}, 
                                      {norAttr, true}, 
                                      {uvAttr, true}});
        mMeshGroup->Load({positions, normals, uvs}, indices);
    }

    mMaterial.mKa = { 0.1f, 0.1f, 0.1f };
    mMaterial.mKd = { 0.8f, 0.8f, 0.8f };
    mMaterial.mKs = { 0.2f, 0.2f, 0.2f };
}

TexturedQuadMesh::~TexturedQuadMesh()
{
    delete mMeshGroup;
}

void TexturedQuadMesh::Render() const
{
    mMeshGroup->Render();
}

}  // namespace engine.