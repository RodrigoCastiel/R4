#include "obj_data.h"

#include <cstdio>
#include <iostream>
#include <QVector2D>
#include <initializer_list>

#include "helper/r4o_file.h"
#include "helper/glb_file.h"
#include "helper/mtlb_file.h"

using namespace engine;

namespace obj_tool
{

ObjData::ObjData()
{
    // Add default object and group (both empty).
    mObjList.push_back({"o_0"});
    mGroups.push_back({0, "g_0", -1, {}, {}});
}

void ObjData::Clear()
{
    mVertices.clear();
    mNormals.clear();
    mUVs.clear();
    mMaterialLib.clear();
    mObjList.clear();

    for (auto & group : mGroups )
    {
        group.mFaces.clear();
    }
    mGroups.clear();

    mObjList.push_back({"o_0"});
    mGroups.push_back({0, "g_0", -1, {}, {}});
}

size_t ObjData::GetNumFaces() const
{
    size_t count = 0;
    for (auto & group : mGroups)
    {
        count += group.mFaces.size();
    }
    return count;
}

void ObjData::LogData() const
{
    std::cout << "#vertices = " << mVertices.size() << std::endl;
    std::cout << "#normals  = " << mNormals.size() << std::endl;
    std::cout << "#uvs      = " << mUVs.size() << std::endl;
    std::cout << "#faces    = " << ObjData::GetNumFaces() << std::endl;
    std::cout << "#materials = " << mMaterialLib.size() << std::endl;
    std::cout << "#objs     = " << mObjList.size() << std::endl;
    std::cout << "#groups   = " << mGroups.size() << std::endl;
    for (auto & group : mGroups)
    {
        std::cout << "\t" << group.mName << " --> '" << mObjList[group.mObjIndex].mName << "';" << std::endl;
    }
}

void ObjData::LogData(std::string & output) const
{
    std::string text = "";

    text.append("#vertices  = " + std::to_string(mVertices.size()) + "\n");
    text.append("#normals   = " + std::to_string(mNormals.size()) + "\n");
    text.append("#uvs       = " + std::to_string(mUVs.size()) + "\n");
    text.append("#faces     = " + std::to_string(ObjData::GetNumFaces()) + "\n");
    text.append("#materials = " + std::to_string(mMaterialLib.size()) + "\n");
    text.append("#objs      = " + std::to_string(mObjList.size()) + "\n");
    text.append("#groups    = " + std::to_string(mGroups.size()) + "\n");
    for (auto & group : mGroups)
    {
        text.append("\t" + group.mName + " --> '" + mObjList[group.mObjIndex].mName 
                         + "'; " + mMaterialLib[group.mMaterialIndex].mName + "\n");
    }

    output = text;
}

void ObjData::AddGroup(const std::string & name)
{
    std::string group_name = name;

    if (name == "")
    {
        group_name = "g_" + std::to_string(GetNumGroups());
    }

    // If current group is empty, then it's the default one -> just update its name.
    if (mGroups.back().mFaces.size() == 0)
    {
        mGroups.back().mName = group_name;
    }
    else  // A new group was started.
    {
        int obj_index = mObjList.size()-1;
        mGroups.push_back({obj_index, group_name, -1, {}, {}});
    }
}

void ObjData::AddObj(const std::string & name)
{
    std::string obj_name = name;

    if (name == "")
    {
        obj_name = "o_" + std::to_string(mObjList.size());
    }

    // If current obj is the default one -> just update its name.
    if (mObjList.back().mName == "o_0")
    {
        mObjList.back().mName = obj_name;
    }
    else  // Start a new obj.
    {
        mObjList.push_back({obj_name});

        // Start a new default group, also.
        int obj_index = mObjList.size()-1;
        std::string group_name = "g_" + std::to_string(mGroups.size());
        mGroups.push_back({obj_index, group_name, -1, {}, {}});
    }
}

bool ObjData::ExportToMeshGroupGLB(const QString & glbFilename, int groupIndex, bool smoothShading)
{
    if ((groupIndex < 0) && (groupIndex > mGroups.size())) 
    {
        // Error: invalid index.
        std::cerr << "ERROR Invalid groupIndex. " << std::endl;
        return false;
    }
    const ObjGroup & objGroup = mGroups[groupIndex];

    if (objGroup.mFaces.size() == 0)
    {
        std::cerr << "ERROR Empty group (ignored). " << std::endl;
        return false;
    }

    // Check attribute list.
    bool hasNormals = (mNormals.size() > 0);
    bool hasUVs     = (objGroup.mFaces[0][0][1] != 0);  // No uv index specified.

    //if (!hasNormals && smoothShading)
    //{
    //    ObjData::ComputeVertexNormals(true);  
    //}
    //else if (!smoothShading)
    //{
    //    ObjData::ComputeFaceNormals(true);
    //}

    // Count number of vertices.
    size_t numVertices = 3 * ObjData::GetNumFacesOnGroup(groupIndex);
    size_t numElements = numVertices;

    // Copy all data referenced on face indices into an auxiliary buffer.
    int bufferSize = (3 + 3*hasNormals + 2*hasUVs)*numVertices;
    std::vector<GLfloat> buffer;
    buffer.reserve(bufferSize);

    // Add vertices to buffer.
    for (int i = 0; i < objGroup.mFaces.size(); i++)
    {
        for (int vtx = 0; vtx < 3; vtx++)
        {
            // Retrieve v, vt and vn.
            int pos_index = objGroup.mFaces[i][vtx][0];
            int  uv_index = objGroup.mFaces[i][vtx][1];
            int nor_index = objGroup.mFaces[i][vtx][2];

            buffer.push_back(mVertices[pos_index].x());
            buffer.push_back(mVertices[pos_index].y());
            buffer.push_back(mVertices[pos_index].z());
        }
    }

    // Add normals to buffer.
    for (int i = 0; i < objGroup.mFaces.size(); i++)
    {
        for (int vtx = 0; vtx < 3; vtx++)
        {
            // Retrieve v, vt and vn.
            int pos_index = objGroup.mFaces[i][vtx][0];
            int  uv_index = objGroup.mFaces[i][vtx][1];
            int nor_index = objGroup.mFaces[i][vtx][2];

            if (objGroup.mFaces[i].UsesSmoothShading())
            {
                buffer.push_back(mNormals[nor_index].x());
                buffer.push_back(mNormals[nor_index].y());
                buffer.push_back(mNormals[nor_index].z());  
            }
            else  // Flat shading: use face normals.
            {
                QVector3D n = objGroup.mFaces[i].GetNormal();
                buffer.push_back(n.x());
                buffer.push_back(n.y());
                buffer.push_back(n.z()); 
            }
        }
    }

    // Add UVs to buffer (if specified).
    if (hasUVs)
    {
        for (int i = 0; i < objGroup.mFaces.size(); i++)
        {
            for (int vtx = 0; vtx < 3; vtx++)
            {
                // Retrieve v, vt and vn.
                int pos_index = objGroup.mFaces[i][vtx][0];
                int  uv_index = objGroup.mFaces[i][vtx][1];
                int nor_index = objGroup.mFaces[i][vtx][2];

                buffer.push_back(mUVs[uv_index].x());
                buffer.push_back(mUVs[uv_index].y());
            }
        }
    }

    // Build up MeshGroup.
    std::vector<GLuint> vertexAttribList;

    // Add position attrib info.
    vertexAttribList.push_back(3);
    vertexAttribList.push_back(3);

    if (hasUVs)
    {
        vertexAttribList.push_back(2);
    }

    // Write GLB file =========================================================
    std::string filenameStd = glbFilename.toStdString();
    const char* filename = filenameStd.c_str();
    FILE* glbFile = fopen(filename, "wb");

    if (glbFile == NULL)
        return false;

    // Prepare header.
    GLB_FileHeader header;
    header._uint_size = sizeof(unsigned);
    header._float_size = sizeof(float);

    memcpy(header._origin_name, objGroup.mName.c_str(), kORIGIN_OBJ_NAME_SIZE);
    header._num_vertices = numVertices;
    header._num_elements = numElements;

    for (int i = 0; i < 4; i++)
        header._vertex_format[i] = 0;

    int numAttributes = std::min<int>((int)vertexAttribList.size(), 4);
    for (int i = 0; i < numAttributes; i++) 
    {
        header._vertex_format[i] = vertexAttribList[i];
    }
    header._draw_mode = GL_TRIANGLES;
    header._num_attributes = numAttributes;

    // Write it.
    fwrite(&header, sizeof(GLB_FileHeader), 1, glbFile);
    fwrite(buffer.data(), sizeof(GLfloat)*bufferSize, 1, glbFile);
    fclose(glbFile);

    return true;
}

bool ObjData::ExportToMTLB(const QString & mtlbFilename)
{
    // Write MTLB file =========================================================
    std::string filenameStd = mtlbFilename.toStdString();
    const char* filename = filenameStd.c_str();
    FILE* mtlbFile = fopen(filename, "wb");

    // Prepare header.
    MTLB_Header mtlb_header;
    memcpy(mtlb_header._mtl_name, mMaterialFilename.c_str(), kMTL_NAME_SIZE);
    mtlb_header._num_materials = mMaterialLib.size();

    // Write header.
    fwrite(&mtlb_header, sizeof(MTLB_Header), 1, mtlbFile);

    // Write materials.
    for (int i = 0; i < mMaterialLib.size(); i++)
    {
        const ObjMaterial & mat = mMaterialLib[i];
        MATERIAL_Header material_header;
        
        for (int k = 0; k < 3; k++) 
        {
            material_header._Ka[k] = mat.mKa[k];
            material_header._Kd[k] = mat.mKd[k];
            material_header._Ks[k] = mat.mKs[k];
        }
        material_header._Ns = mat.mNs;
        material_header._d = mat.mD;
        material_header._illum = mat.mIllum;
        material_header._num_textures = 4;

        std::vector<std::string> textures(4);
        textures[0] = mat.mMapKd;
        textures[1] = mat.mMapKs;
        textures[2] = mat.mMapBump;
        textures[3] = mat.mMapD;

        // Write material header and textures.
        fwrite(&material_header, sizeof(MATERIAL_Header), 1, mtlbFile);

        for (int k = 0; k < textures.size(); k++)
        {
            int texNameSize = textures[k].size();
            fwrite(&texNameSize, sizeof(int), 1, mtlbFile);
            fwrite(textures[k].c_str(), sizeof(char) * texNameSize, 1, mtlbFile);
        }
    }

    fclose(mtlbFile);
    return true;
}

bool ObjData::ExportToR4O(const QString & baseFolderPath, const QString & groupsFolderName)
{
    // Compute flat and smooth normals if necessary.
    bool hasNormals = (mNormals.size() > 0);
    if (mNormals.size() == 0)
        ObjData::ComputeVertexNormals(true);
    ObjData::ComputeFaceNormals(true);

    QString r4oFilepath = baseFolderPath + "/" + groupsFolderName + ".r4o";
    QString mtlbFileName = baseFolderPath + "/" + QString::fromStdString(mMaterialFilename) + "b";

    // Export groups.
    for (int i = 0; i < mGroups.size(); i++) 
    {
        ObjData::ExportToMeshGroupGLB(baseFolderPath + "/" + groupsFolderName + 
            "/g" + QString::number(i) + ".glb", i, false);
    }

    // Export to mtlb.
    QString mtlbFilePath = baseFolderPath + "/" + QString::fromStdString(mMaterialFilename) + "b";
    std::string materialFilename = mMaterialFilename;
    if (!ObjData::ExportToMTLB(mtlbFilePath))
    {
        std::cerr << "ERROR Couldn't export to MTLB at \'" << mtlbFilePath.toStdString() << "\'" << std::endl;
        materialFilename  = "<ERROR-MTLB>";
    }

    // Prepare r4o file.
    R4OFile r4oFile;
    r4oFile.MTLBFileName() = mMaterialFilename + "b";
    r4oFile.GroupsBaseFolder() = groupsFolderName.toStdString();
    r4oFile.GetNumGroups() = mGroups.size();

    // Build a list for counting obj's children.
    std::vector<int> objs(mObjList.size(), 0);
    for (int i = 0; i < mGroups.size(); i++)
        objs[mGroups[i].mObjIndex]++;
    int group = 0;
    for (int i = 0; i < mObjList.size(); i++)
    {
        r4oFile.ObjNames().push_back(mObjList[i].mName.c_str());
        r4oFile.ObjGroupIntervals().push_back({group, group+objs[i]-1});
        group += objs[i];
    }

    // Write .r4o.
    if (!r4oFile.Write(r4oFilepath.toStdString()))
        return false;
    
    return true;
}

void ObjData::TriangulateQuads()
{
    for (auto & group : mGroups)
    {
        size_t originalNumFaces = group.mFaces.size();
        for (int i = 0; i < originalNumFaces; i++)
        {
            // If the current face is a quad, then tesselate it.
            if (group.mFaces[i].GetNumVertices() == 4)
            {
                // Remove vertex 3 from current face.
                std::vector<int> vtx3 = group.mFaces[i][3];
                group.mFaces[i].PopVertex();

                //  Create a new triangle with vertices 0, 2, 3.
                std::vector<int> & vtx2 = group.mFaces[i][2];
                std::vector<int> & vtx1 = group.mFaces[i][0];

                group.mFaces.push_back({vtx1, vtx2, vtx3});
            }
        }
    }
}

void ObjData::ComputeVertexNormals(bool normalize)
{
    // For each vtx, stores a list of faces that share it.
    std::vector<std::vector<Face*>> mVertexFaces(mVertices.size());

    // Build up the adjacency list (between vertices and faces).
    for (auto & group : mGroups)
    {
        for (auto & face : group.mFaces)
        {
            for (int vtx = 0; vtx < face.GetNumVertices(); vtx++)
            {
                mVertexFaces[face[vtx][0]].push_back(&face);
            }
        }
    }

    // Compute face normals (but don't normalize them).
    ObjData::ComputeFaceNormals(false);
    mNormals.reserve(mVertices.size());

    // Once the adjacency list is built, then compute the weighted average of all normals whose
    // face share the current vertex.
    for (int i = 0; i < mVertexFaces.size(); i++)
    {
        QVector3D n(0.0f, 0.0f, 0.0f);
        float total_weight = 0.0f;

        for (int j = 0; j < mVertexFaces[i].size(); j++)
            total_weight += mVertexFaces[i][j]->GetNormal().length();

        for (int j = 0; j < mVertexFaces[i].size(); j++)
        {
            float weight = mVertexFaces[i][j]->GetNormal().length();
            n += mVertexFaces[i][j]->GetNormal() * (weight/total_weight);
        }

        if (normalize)
            n.normalize();

        mNormals.push_back(n);

        for (int j = 0; j < mVertexFaces[i].size(); j++)
        {
            Face & face = *mVertexFaces[i][j];
            for (int vtx = 0; vtx < face.GetNumVertices(); vtx++)
            {
                // Point vertex normal to mNormal[i].
                if (face[vtx][0] == i)
                    face[vtx][2] = i;
            }
        }

    }
}

void ObjData::ComputeFaceNormals(bool normalize)
{
    for (auto & group : mGroups)
    {
        for (auto & face : group.mFaces)
        {
            // Triangle!
            if (face.GetNumVertices() == 3)
            {
                QVector3D & v0 = mVertices[face[0][0]];
                QVector3D & v1 = mVertices[face[1][0]];
                QVector3D & v2 = mVertices[face[2][0]];
                QVector3D n = QVector3D::crossProduct(v1-v0, v2-v0);

                if (normalize)
                    n.normalize();

                face.SetNormal(n);
            }
        }
    }
}

QVector3D ObjData::GetCenter()
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    for (auto & vertex : mVertices)
    {
        x += vertex.x();
        y += vertex.y();
        z += vertex.z();
    }

    if (mVertices.size() > 0)
    {
        x /= mVertices.size();
        y /= mVertices.size();
        z /= mVertices.size();
    }

    return {(float)x, (float)y, (float)z};
}

void ObjData::MoveCenterToOrigin()
{
    QVector3D center = ObjData::GetCenter();

    for (auto & vertex : mVertices)
    {
        vertex.setX(vertex.x() - center.x());
        vertex.setY(vertex.y() - center.y());
        vertex.setZ(vertex.z() - center.z());
    }
}

}  // namespace obj_tool.