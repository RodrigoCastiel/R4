#include "mesh_group.h"

#include <cassert>
#include <cstdio>

#include "helper/glb_file.h"

namespace engine
{

MeshGroup::MeshGroup(int numVertices, int numElements, GLenum drawMode, GLenum dataUsage)
: mNumVertices(numVertices)
, mNumElements(numElements)
, mDataUsage(dataUsage)
, mDrawMode(drawMode)
{

}

MeshGroup::MeshGroup(GLenum dataUsage)
    : mNumVertices(0)
    , mNumElements(0)
    , mDataUsage(dataUsage)
    , mDrawMode(GL_TRIANGLES)
{

}

MeshGroup::~MeshGroup() 
{
  MeshGroup::ClearBuffers();
}

void MeshGroup::Render(unsigned renderingPass, unsigned subGroupIndex) const
{
  const int option = renderingPass;
  glCore->glBindVertexArray(mVaoList[option]);
  glCore->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEabList[subGroupIndex]);  // ...

  glCore->glDrawElements(
    mDrawMode,         // mode (GL_LINES, GL_TRIANGLES, ...)
    mNumElements,      // number of vertices.
    GL_UNSIGNED_INT,   // type.
    (void*)0           // element array buffer offset.
   );
}

void MeshGroup::SetVertexAttribList(std::vector<GLuint> && vertexAttribList)
{
    mVertexAttributeList = vertexAttribList;

    mVertexSize = 0;
    mNumAttributes = 0;
    for (GLuint attribSize : mVertexAttributeList) 
    {
        mVertexSize += attribSize;
        mNumAttributes++;
    }

    // Generate geometry buffers.
    glCore->glGenBuffers(1, &mVbo);       // Vertex buffer object.
}

void MeshGroup::SetVertexAttribList(const std::vector<GLuint> & vertexAttribList)
{
    mVertexAttributeList = vertexAttribList;

    mVertexSize = 0;
    mNumAttributes = 0;
    for (GLuint attribSize : mVertexAttributeList) 
    {
        mVertexSize += attribSize;
        mNumAttributes++;
    }

    // Generate geometry buffers.
    glCore->glGenBuffers(1, &mVbo);       // Vertex buffer object.
}

void MeshGroup::SetVertexAttribList(std::initializer_list<GLuint> vertexAttribList)
{
    mVertexAttributeList = std::vector<GLuint>(vertexAttribList);

    mVertexSize = 0;
    mNumAttributes = 0;
    for (GLuint attribSize : mVertexAttributeList) 
    {
        mVertexSize += attribSize;
        mNumAttributes++;
    }

    // Generate geometry buffers.
    glCore->glGenBuffers(1, &mVbo);       // Vertex buffer object.
}

int MeshGroup::AddRenderingPass(const std::vector<std::pair<GLint, bool>> & attribList)
{
  assert(attribList.size() == mNumAttributes);

  GLuint vao = 0;
  glCore->glGenVertexArrays(1, &vao);

  glCore->glBindVertexArray(vao);
  glCore->glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  
  MeshGroup::BuildVAO(attribList);

  for (int i = 0; i < mNumAttributes; i++)
  {
    const GLuint attribLoc = attribList[i].first;
    const bool active = attribList[i].second;

    // If the attribute is active...
    if (active)
    {
      glCore->glEnableVertexAttribArray(attribLoc);
    }
    else if (attribLoc != -1)
    {
      glCore->glDisableVertexAttribArray(attribLoc);
    }
  }

  mVaoList.push_back(vao);

  return mVaoList.size()-1;
}

void MeshGroup::AllocateVBO(const GLfloat* vertices)
{  
    // Allocate buffer for vertices (VBO).
    glCore->glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glCore->glBufferData(GL_ARRAY_BUFFER, mVertexSize * mNumVertices * sizeof(GLfloat),
                vertices, mDataUsage);
}

void MeshGroup::AllocateNewEAB(const GLuint* elements, int numElements)
{
    // Generate new EAB and add to EAB list.
    GLuint eab = 0;
    glCore->glGenBuffers(1, &eab);
    mEabList.push_back(eab);

    // Allocate buffer for elements (EAB).
    glCore->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
    glCore->glBufferData(GL_ELEMENT_ARRAY_BUFFER, numElements * sizeof(GLuint),
                         elements, mDataUsage);
}

void MeshGroup::ClearBuffers()
{
  glCore->glDeleteBuffers(1, &mVbo);
  glCore->glDeleteBuffers(1, &mEab);  // ...
  glCore->glDeleteBuffers(mEabList.size(), mEabList.data());
  glCore->glDeleteVertexArrays(mVaoList.size(), mVaoList.data());
}

bool MeshGroup::Load(const GLfloat* buffer, const GLuint* indices)
{
  // Reserve vertex buffer and initialize element array (indices array).
  if (indices)  // Element array provided.
  {
    MeshGroup::AllocateVBO(buffer);
    MeshGroup::AllocateNewEAB(indices, mNumElements);
  }
  else  // Element array wasn't provided -- build it up.
  {
    std::vector<GLuint> elementsBuffer(mNumElements);

    for (int i = 0; i < mNumElements; i++)
      elementsBuffer[i] = i;

    MeshGroup::AllocateVBO(buffer);                                  // Unique VBO.
    MeshGroup::AllocateNewEAB(elementsBuffer.data(), mNumElements);  // Default EAB.
  }
  return true;
}

bool MeshGroup::Load(const std::vector<GLfloat*> & bufferList, const GLuint* indices)
{
  assert(bufferList.size() == mNumAttributes);

  // Reserve vertex buffer and initialize element array (indices array).
  if (indices)  // Element array provided.
  {
    MeshGroup::AllocateVBO(nullptr);  // nullptr -> generate but do not send data.
    MeshGroup::AllocateNewEAB(indices, mNumElements);  // Default EAB.
  }
  else  // Element array wasn't provided -- build it up.
  {
    std::vector<GLuint> elementsBuffer(mNumElements);

    for (int i = 0; i < mNumElements; i++)
      elementsBuffer[i] = i;

    MeshGroup::AllocateVBO(nullptr);
    MeshGroup::AllocateNewEAB(elementsBuffer.data(), mNumElements);  // Default EAB.
  }

  MeshGroup::Update(bufferList);
  return true;
}

bool MeshGroup::Load(const GLfloat* buffer)
{
    // Reserve vertex buffer.
    MeshGroup::AllocateVBO(buffer);
    return true;
}

bool MeshGroup::Load(const std::vector<GLfloat*> & bufferList)
{
    MeshGroup::AllocateVBO(nullptr);  // nullptr -> generate but do not send data.
    MeshGroup::Update(bufferList);
    return true;
}

bool MeshGroup::Update(const GLfloat* buffer)
{
    glCore->glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glCore->glBufferSubData(GL_ARRAY_BUFFER, 0, mVertexSize * mNumVertices * sizeof(GLfloat), buffer);
    return true;
}

bool MeshGroup::Update(const std::vector<GLfloat*> & bufferList)
{
  assert(bufferList.size() == mNumAttributes);

  glCore->glBindBuffer(GL_ARRAY_BUFFER, mVbo);

  // Upload subdata of geometry to GPU.
  int offset = 0;
  for (int j = 0; j < mNumAttributes; j++)
  {
    const unsigned size = mVertexAttributeList[j];
    const float* buffer = bufferList[j];

    if ((size > 0) && (buffer != nullptr))
    {
      glCore->glBufferSubData(GL_ARRAY_BUFFER, offset, size*mNumVertices*sizeof(GLfloat), buffer);
    }

    offset += size*mNumVertices * sizeof(GLfloat);
  }

  return true;
}

void MeshGroup::BuildVAO(const std::vector<std::pair<GLint, bool>> & attribList)
{
  // Specify VAO.
  int offset = 0;
  for (int j = 0; j < mNumAttributes; j++)
  {
    const int size    = mVertexAttributeList[j];
    const GLint loc   = attribList[j].first;
    const bool active = attribList[j].second;

    if ((size > 0) && active)
    {
      // Specify internal storage architecture of Vertex Buffer.
      glCore->glVertexAttribPointer(loc, size, GL_FLOAT, GL_FALSE, 0, 
        (void*)(sizeof(GLfloat) * offset*mNumVertices));
    }

    offset += size;
  }
}

bool MeshGroup::LoadGLB(const std::string & glbFilename)
{
    // Read GLB file =========================================================
    const char* filename = glbFilename.c_str();
    FILE* glbFile = fopen(filename, "rb");

    if (glbFile == NULL)
        return false;

    // Read header.
    GLB_FileHeader header;
    fread(&header, sizeof(GLB_FileHeader), 1, glbFile);

    mNumVertices = header._num_vertices;
    mNumElements = header._num_elements;
    mDrawMode = header._draw_mode;

    // Build element array.
    std::vector<GLuint> elements(mNumElements);
    for (int i = 0; i < mNumElements; i++)
        elements[i] = i;

    // Build vertex attribute list.
    std::vector<GLuint> attributeList;
    int numAttributes = header._num_attributes;
    for (int i = 0; i < numAttributes; i++) 
    {
        attributeList.push_back(header._vertex_format[i]);
    }
    MeshGroup::SetVertexAttribList(attributeList);  // Vertex format.

    // Read buffer.
    int bufferSize = this->GetVertexSize() * mNumVertices;
    std::vector<GLfloat> buffer(bufferSize);
    fread(buffer.data(), sizeof(GLfloat)*bufferSize, 1, glbFile);

    // Read list of subgroups (each one has a different material).
    int numSubGroups = 0;
    fread(&numSubGroups, sizeof(int), 1, glbFile);

    std::vector<int> startElements;

    for (int i = 0; i < numSubGroups; i++) 
    {
        int faceIndex = 0;
        int materialIndex = 0;
        fread(&faceIndex, sizeof(int), 1, glbFile);
        fread(&materialIndex, sizeof(int), 1, glbFile);
        mMaterialIndex = materialIndex;

        mMaterialIndexList.push_back(materialIndex);
        startElements.push_back(faceIndex*3);
    }

    for (int i = 0; i < numSubGroups - 1; i++)
    {
        int numElements = startElements[i + 1] - startElements[i];
        MeshGroup::AllocateNewEAB(&elements[startElements[i]], numElements);
    }

    // Handle last subgroup.
    if (numSubGroups > 0)
    {
        int numElements = mNumElements - startElements[numSubGroups-1];
        MeshGroup::AllocateNewEAB(&elements[startElements[numSubGroups-1]], numElements);
    }

    fclose(glbFile);

    // Upload geometry to graphics card.
    MeshGroup::AllocateVBO(buffer.data());  // nullptr -> generate but do not send data.

    return true;
}


}  // namespace engine.