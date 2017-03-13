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
    { }

    void PushVertex(int v=0, int vt=0, int vn=0, int vtan=0)
    {
        mVertexList.push_back({v, vt, vn});
    }

    void PopVertex()
    {
        mVertexList.pop_back();
    }

    inline int GetNumVertices() const { return mVertexList.size(); }

    const std::vector<int> & operator[](int index) const { return mVertexList[index]; }
    std::vector<int> & operator[](int index) { return mVertexList[index]; }

    QVector3D GetNormal() const { return mNormal; }
    void SetNormal(const QVector3D & normal) { mNormal = normal; }

    bool UsesSmoothShading() const { return mSmoothShading; }
    void SetSmoothShading(bool smoothShading) { mSmoothShading = smoothShading; }
    
private:
    std::vector<std::vector<int>> mVertexList;
    QVector3D mNormal;
    bool mSmoothShading;
};


}  // namespace obj_tool.