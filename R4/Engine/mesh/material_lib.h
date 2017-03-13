// +--------------------------------------------+
// | File: material_lib.h                       |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-05.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <vector>
#include <QVector3D>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>

namespace engine
{

struct MaterialUniformPack
{
    GLint mKaLoc;
    GLint mKdLoc;
    GLint mKsLoc;

    float mShininess { 1.0f };
};

struct Material
{
    QVector3D mKa;  // Ambient component.
    QVector3D mKd;  // Diffuse component.
    QVector3D mKs;  // Specular component.

    float mShininess { 1.0f };
    float mOpacity   { 1.0f };
    int mIllum       {   2  };

    QOpenGLTexture* mDiffuseMap   { nullptr };
    QOpenGLTexture* mSpecularMap  { nullptr };      // Not supported yet.
    QOpenGLTexture* mNormalMap    { nullptr };
    QOpenGLTexture* mTransparencyMap  { nullptr };  // Not supported yet.
};

class MaterialLib
{
public:
    // +++ Constructors/Destructors +++ ---------------------------------------

    MaterialLib();
    ~MaterialLib();

    // +++ Loading / Settings +++ ---------------------------------------------

    // Loads from .mtlb file. 
    // Please provide the full path and also the containing folder.
    bool LoadMTLB(const std::string & filePath, const std::string & folderPath);

    // Returns the number of materials stored in the list.
    inline int GetNumMaterials() const { return mMaterialList.size(); }

    // Returns material at 'index'.
    inline const Material & GetMaterial(int index) const { return mMaterialList[index]; }
    void SetMaterial(const Material & material) { mMaterialList.push_back(material); }

    inline void AddMaterial(const Material & material) { mMaterialList.push_back(material); }

private:
    std::string mFilename;
    std::vector<Material> mMaterialList;
};

}  // namespace engine.