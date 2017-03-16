// +--------------------------------------------+
// | File: obj_face.h                           |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-09.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <QVector3D>

namespace obj_tool
{

class Face
{
public:
    Face() 
    { }

    Face(std::initializer_list<std::vector<int>> vertexList) 
        : mVertexList(vertexList)
        , mNormal({.0f, .0f, .0f})
        , mSmoothShading(false)
        , mMaterialIndex(0)
    { }

    void PushVertex(int v=0, int vt=0, int vn=0, int vtan=0)
    {
        mVertexList.push_back({v, vt, vn});
    }

    void PopVertex()
    {
        mVertexList.pop_back();
    }

    int GetNumVertices() const { return mVertexList.size(); }

    const std::vector<int> & operator[](int index) const { return mVertexList[index]; }
    std::vector<int> & operator[](int index) { return mVertexList[index]; }

    QVector3D GetNormal() const { return mNormal; }
    void SetNormal(const QVector3D & normal) { mNormal = normal; }

    bool UsesSmoothShading() const { return mSmoothShading; }
    void SetSmoothShading(bool smoothShading) { mSmoothShading = smoothShading; }
    
    int GetMaterialIndex() const { return mMaterialIndex; }
    void SetMaterialIndex(int materialIndex) { mMaterialIndex = materialIndex; }

    void SetVertexList(const std::vector<std::vector<int>> & vertexList) { mVertexList = vertexList; }

private:
    std::vector<std::vector<int>> mVertexList;
    QVector3D mNormal;
    bool mSmoothShading;
    int mMaterialIndex;
};


}  // namespace obj_tool.