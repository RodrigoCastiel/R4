#include "static_object.h"

#include <iostream>

#include "helper/r4o_file.h"
#include "renderer/phong_renderer.h"

namespace engine
{

StaticObject::~StaticObject()
{
    for (int i = 0; i < mGroups.size(); i++)
    {
        delete mGroups[i];
    }
}

bool StaticObject::LoadFromR4O(const std::string & filePath)
{
    // Find last '/' or '\\' to retrieve containing folder.
    std::string folderPath = ".";
    size_t slash_pos = filePath.find_last_of("\\/");

    if (slash_pos != std::string::npos)  // Found.
        folderPath = filePath.substr(0, slash_pos);

    R4OFile r4oFile;
    if (!r4oFile.Load(filePath))
        return false;

    // Load MTLB file.
    std::string mtlbPath = folderPath + "/" + r4oFile.MTLBFileName();
    if (!mMaterialLib.LoadMTLB(mtlbPath, folderPath))
        std::cerr << "WARNING MTLB not loaded ('" << mtlbPath << "')\n";

    // Load groups.  TODO: MULTITHREADING!
    mGroups.reserve(r4oFile.GetNumGroups());
    for (int i = 0; i < r4oFile.GetNumGroups(); i++)
    {
        // Load group.
        std::string glbFilename = folderPath + "/" + r4oFile.GroupsBaseFolder() + "/g" + std::to_string(i) + ".glb";
        MeshGroup* group = new MeshGroup();
        bool loaded = group->LoadGLB(glbFilename);

        if (!loaded) 
        {
            std::cerr << "WARNING Group buffer .glb not loaded ('" << glbFilename << "')\n";
            continue;
        }
        
        // Add full-rendering pass.
        const int numAttributes = group->GetVertexAttribList().size();    
        
        // Optimized version:
        if (numAttributes == 2)
            group->AddRenderingPass({{0, true}, {1, true}});
        else if (numAttributes == 3)
            group->AddRenderingPass({{0, true}, {1, true}, {2, true}});
        else if (numAttributes == 4)
            group->AddRenderingPass({{0, true}, {1, true}, {2, true}, {3, true}});
        
        //testMesh->AddRenderingPass({ {phong_posAttr, true}, {phong_norAttr, true}, {phong_texAttr, true} });

        mGroups.push_back(group);
    }

    return true;
}

void StaticObject::Render(PhongRenderer* renderer) const
{
    // Iterate over groups and subgroups.
    for (int i = 0; i < mGroups.size(); i++)
    {
        for (int j = 0; j < mGroups[i]->GetNumSubGroups(); j++) 
        {
            int materialIndex =  mGroups[i]->GetMaterialIndex(j);
            const Material & mat = mMaterialLib.GetMaterial(materialIndex);
            renderer->SetMaterial(mat);
            mGroups[i]->Render(0, j);
        }
    }
}

}  // namespace engine.