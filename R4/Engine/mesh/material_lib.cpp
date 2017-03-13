#include "material_lib.h"

#include <cassert>
#include <cstdio>

#include "helper/mtlb_file.h"

namespace engine
{

MaterialLib::MaterialLib()
{

}

MaterialLib::~MaterialLib()
{
    for (auto & material : mMaterialList)
    {
        delete material.mDiffuseMap;
        delete material.mSpecularMap;
        delete material.mNormalMap;
        delete material.mTransparencyMap;
    }
}

bool MaterialLib::LoadMTLB(const std::string & filePath, const std::string & folderPath)
{
    const char* filename = filePath.c_str();
    FILE* mtlbFile = fopen(filename, "rb");

    if (mtlbFile == NULL)
        return false;

    // Read header.
    MTLB_Header header;
    QOpenGLTexture* textureList[5];

    fread(&header, sizeof(MTLB_Header), 1, mtlbFile);
    mFilename = std::string(header._mtl_name);

    mMaterialList.clear();
    mMaterialList.resize(header._num_materials);

    // Read materials.
    for (int i = 0; i < mMaterialList.size(); i++)
    { 
        MATERIAL_Header material_header;
        fread(&material_header, sizeof(MATERIAL_Header), 1, mtlbFile);
        char buffer[256];

        // Ka, Ks, Ks.
        mMaterialList[i].mKa[0] = material_header._Ka[0];
        mMaterialList[i].mKd[0] = material_header._Kd[0];
        mMaterialList[i].mKs[0] = material_header._Ks[0];

        mMaterialList[i].mKa[1] = material_header._Ka[1];
        mMaterialList[i].mKd[1] = material_header._Kd[1];
        mMaterialList[i].mKs[1] = material_header._Ks[1];

        mMaterialList[i].mKa[2] = material_header._Ka[2];
        mMaterialList[i].mKd[2] = material_header._Kd[2];
        mMaterialList[i].mKs[2] = material_header._Ks[2];

        // Ns, d and illumination type.
        mMaterialList[i].mOpacity = material_header._d;
        mMaterialList[i].mShininess = material_header._Ns;
        mMaterialList[i].mIllum = material_header._illum;
            
        // Read textures.
        for (int j = 0; j < material_header._num_textures; j++)
        {
            int length = 0;
            fread(&length, sizeof(int), 1, mtlbFile);
            fread(buffer, sizeof(char)*length, 1, mtlbFile);
            buffer[length] = '\0';

            textureList[j] = nullptr;
            if (length > 0)
            {
                QString folder;
                if (folderPath == "")
                    folder = ".";
                QString imgPath = QString::fromStdString(folderPath) + "/" + QString(buffer);
                QImage img = QImage(imgPath).mirrored();

                if (!img.isNull())
                {
                    textureList[j] = new QOpenGLTexture(img);
                    textureList[j]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
                    textureList[j]->setMagnificationFilter(QOpenGLTexture::Linear);
                }
            }
        }

        // Textures (load them). Uses order specified on 'mtlb_file.h'
        mMaterialList[i].mDiffuseMap  = textureList[0];
        mMaterialList[i].mSpecularMap = textureList[1];
        mMaterialList[i].mNormalMap   = textureList[2];
        mMaterialList[i].mTransparencyMap = textureList[3];
    }

    fclose(mtlbFile);

    return true;
}


}  // namespace engibe.