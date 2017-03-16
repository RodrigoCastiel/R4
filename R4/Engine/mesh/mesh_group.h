// +--------------------------------------------+
// | File: mesh_group.h                         |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-05.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <vector>
#include <cassert>
#include <initializer_list>
#include <QOpenGLContext>

#include "core/gl_core.h"

namespace engine
{

const std::pair<GLint, bool> kNoAttrib = {-1, false};

class MeshGroup
{
public:
    /* Most used methods */
    MeshGroup(int numVertices, int numElements, GLenum drawMode = GL_TRIANGLE_STRIP,
        GLenum dataUsage = GL_STATIC_DRAW);
    MeshGroup(GLenum dataUsage = GL_STATIC_DRAW);

    ~MeshGroup();

    // Specifies which data/properties the vertices contain.
    void SetVertexAttribList(std::vector<GLuint> && vertexAttribList);
    void SetVertexAttribList(const std::vector<GLuint> & vertexAttribList);
    void SetVertexAttribList(std::initializer_list<GLuint> vertexAttribList);

    // Adds a different way of rendering the object - each one might use different 
    // attributes of the vertex. The active attribute list specifies which attributes 
    // are enabled and their corresponding shader locations.
    int AddRenderingPass(const std::vector<std::pair<GLint, bool>> & attribList);

    // Should be called on display function (it calls glDrawElements for subGroupIndex EAB).
    void Render(unsigned renderingPass = 0, unsigned subGroupIndex = 0) const;

    // Loads VBO from 'buffer'.
    bool Load(const GLfloat* buffer);

    // Loads VBO from list of buffers (sub-buffers).
    bool Load(const std::vector<GLfloat*> & bufferList);

    // Loads VBO from 'buffer' and EAB from 'indices'.
    bool Load(const GLfloat* buffer, const GLuint* indices);

    // Loads VBO from list of buffers and EAB from 'indices'.
    bool Load(const std::vector<GLfloat*> & bufferList, const GLuint* indices);

    // Loads VBO and EAB from binary file.
    // '.glb' => graphics library buffer binary file.
    // Automatically sets vertex attribute list.
    bool LoadGLB(const std::string & glbFilename);

    // TODO: document.
    bool Update(const GLfloat* buffer);
    bool Update(const std::vector<GLfloat*> & bufferList);

    // Getters.
    inline GLuint GetNumVertices() const { return mNumVertices; }
    inline GLuint GetNumElements() const { return mNumElements; }
    inline GLuint GetVertexSize() const { return mVertexSize; }
    inline GLenum GetDataUsage() const { return mDataUsage; }
    inline GLenum GetDrawMode()  const { return mDrawMode; }

    // Returns the number of sub-groups (a subset of faces that use the same material).
    inline GLuint GetNumSubGroups() const { return mEabList.size(); }

    // Returns the material index of the subgroup.
    int GetMaterialIndex(int subGroupIndex) const { return mMaterialIndexList[subGroupIndex]; }

    // Get vertex format:
    // v, vn, vt2d = {3, 3, 2}
    // v, vn, vt2d, vtan = {3, 3, 2, 3}
    // ...
    inline const std::vector<GLuint> & GetVertexAttribList() const { return mVertexAttributeList; }

    // Setters.
    void SetDrawMode(GLenum drawMode) { mDrawMode = drawMode; }

    // XXX.
    int GetMaterialIndex() { return mMaterialIndex; }

private:
    // Specifies vertex attribute object (how attributes are spatially stored into VBO and
    // mapped to attribute locations on shader).
    void BuildVAO(const std::vector<std::pair<GLint, bool>> & attribList);

    // Generate buffers on GPU (VBO).
    void AllocateVBO(const GLfloat* vertices);

    // Generates a new EAB.
    void AllocateNewEAB(const GLuint* elements, int numElements);

    // Destroys buffers on GPU (VAO, VBO, EAB).
    void ClearBuffers();

    /* Attributes */

    // OpenGL buffer IDs.
    GLuint mEab { 0 };  // Element array buffer.
    GLuint mVbo { 0 };  // Vertex buffer object.
    std::vector<GLuint> mVaoList;  // Verter array object list.
    std::vector<GLuint> mEabList;  // Element array buffer (for subgroups with different materials)

    // Mesh attributes.
    GLenum mDrawMode;     // How mesh is rendered (drawing mode).
    GLenum mDataUsage;    // Tells if buffers are dynamic/static or for reading/writing.

    GLuint mNumVertices;  // Number of vertices in this group.
    GLuint mNumElements;  // Number of elements (indices of vertex).

    GLuint mVertexSize    { 0 };  // Number of floating points stored per vertex.
    GLuint mNumAttributes { 0 };  // Number of attributes.

    // Vertex attributes descriptor -> specifies which attributes a vertex contain and also
    // their dimensionality and order. This is constant within the lifetime of a MeshGroup.
    std::vector<GLuint> mVertexAttributeList;

    // Material index list (one for each subgroup).
    std::vector<int> mMaterialIndexList;
    int mMaterialIndex { 0 };
};

}  // namespace gloo.