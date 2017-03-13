// +--------------------------------------------+
// | File: mtlb_file.h                          |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-13.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <vector>
#include <string>
#include <utility>

namespace engine
{

struct R4OFile
{
    // I/O.
    bool Load(const std::string & iFilePath);
    bool Write(const std::string & oFilePath);

    // Getters.
    inline int & GetNumGroups() { return mNumGroups; }
    inline std::string & Architecture() { return mArchitecture; }
    inline std::string & MTLBFileName() { return mMtlbFilename; }
    inline std::string & GroupsBaseFolder() { return mGroupsBaseFolder; }
    inline std::vector<std::string> & ObjNames() { return mObjNames; }
    inline std::vector<std::pair<int, int>> & ObjGroupIntervals() { return mObjGroupIntervals; }

    // Data.
    int mNumGroups;
    std::string mArchitecture;
    std::string mMtlbFilename;
    std::string mGroupsBaseFolder;
    std::vector<std::string> mObjNames;
    std::vector<std::pair<int, int>> mObjGroupIntervals;
};

}  // namespace engine.