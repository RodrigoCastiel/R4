// +--------------------------------------------+
// | File: obj_data.h                           |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-09.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

//  ObjData contains all data specified by an .obj file.
//  It's a temporary data structure. It can be export to
//  StaticObject, RiggedObject and so on.

#pragma once

#include "mesh/mesh_group.h"

#include "obj_face.h"
#include "obj_material.h"

namespace obj_tool
{

// Group specified by 'g' tolken.
struct ObjGroup
{
    int mObjIndex;
    std::string mName;
    int mMaterialIndex;
    std::vector<Face> mFaces;
    std::vector<int> mVertexFormat;
};

// Set of groups specified by 'o' tolken.
struct SubObj
{
    std::string mName;
};

class ObjData
{
public:
    ObjData();

    void Clear();

    void AddMaterial(const ObjMaterial & material);

    void AddVertex(const QVector3D & attrib);
    void AddNormal(const QVector3D & attrib);
    void AddUV(const QVector2D & attrib);

    void AddFace(const Face & face);
    void AddGroup(const std::string & name);
    void AddObj(const std::string & name);

    void LogData() const;
    void LogData(std::string & output) const;

    void SetMaterialIndexForCurrentGroup(int index);
    void SetObjFilename(const std::string & objFilename) { mObjFilename = objFilename; }
    void SetMaterialFilename(const std::string & materialFilename) { mMaterialFilename = materialFilename; }

    inline const std::string & GetObjFilename() const { return mObjFilename; }
    inline const std::string & GetMaterialFilename() const { return mMaterialFilename; }

    std::vector<QVector3D> & GetVertices() { return mVertices; }
    std::vector<QVector3D> & GetNormals()  { return mNormals;  }
    std::vector<QVector2D> & GetUVs()      { return mUVs;      }
    std::vector<ObjMaterial> & GetMaterialLib() { return mMaterialLib; }

    // Getter methods.
    inline size_t GetNumVertices() const { return mVertices.size(); }
    inline size_t GetNumNormals()  const { return mNormals.size();  }
    inline size_t GetNumUVs()      const { return mUVs.size();      }
    inline size_t GetNumObjs()     const { return mObjList.size(); }

    inline size_t GetNumGroups() const { return mGroups.size(); }
    inline size_t GetNumFacesOnGroup(int index) const { return mGroups[index].mFaces.size(); }
    size_t GetNumFaces() const;

    // Builds a renderable mesh group from the topology data of ObjMesh.
    // If an invalid groupIndex is passed, returns false.
    // NOTE: assumes that both flat and smooth normals are computed.
    bool ExportToMeshGroupGLB(const QString & glbFilename, int groupIndex, int forceNormals);

    bool ExportToR4O(const QString & baseFolderPath, const QString & groupsFolderName, int forceNormals);

    bool ExportToMTLB(const QString & mtlbFilename);

    // Geometry processing methods.

    // Triangulates all quads on the surface mesh. Other types of polygons are ignored.
    void TriangulateQuads();

    // Calculates the normal vector for each vertex on the surface.
    // The mesh must be triangulated.
    void ComputeVertexNormals(bool normalize=true);

    // Calculates the normal vector for each face. It considers the faces to be triangular.
    void ComputeFaceNormals(bool normalize=true);

    // Returns centroid of model (using vertices only).
    QVector3D GetCenter();

    // Moves all vertices in such a way that the center stays at the origin.
    void MoveCenterToOrigin();
    

private:
    // General data.
    std::string mObjFilename;
    std::string mMaterialFilename;

    std::vector<QVector3D> mVertices;
    std::vector<QVector3D> mNormals;
    std::vector<QVector2D> mUVs;

    // List of groups.
    std::vector<ObjGroup> mGroups;

    // List of objects.
    std::vector<SubObj> mObjList;

    // Material Library.
    std::vector<ObjMaterial> mMaterialLib;
};

// ----------------------------------------------------------------------------

inline 
void ObjData::SetMaterialIndexForCurrentGroup(int index)
{
    mGroups.back().mMaterialIndex = index;
}

inline
void ObjData::AddMaterial(const ObjMaterial & material)
{
    mMaterialLib.push_back(material);
}

inline
void ObjData::AddVertex(const QVector3D & attrib)
{ 
    mVertices.push_back(attrib); 
}

inline 
void ObjData::AddNormal(const QVector3D & attrib)
{ 
    mNormals.push_back(attrib);
}

inline
void ObjData::AddUV(const QVector2D & attrib)
{ 
    mUVs.push_back(attrib); 
}

}  // namespace obj_tool.