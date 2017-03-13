#include "r4o_file.h"

#include <cstdio>
#include <sstream>
#include <fstream>

namespace engine
{

bool R4OFile::Write(const std::string & oFilePath)
{
    FILE* r4oFile = fopen(oFilePath.c_str(), "w");

    if (r4oFile == nullptr)  // Couldn't open file.
        return false;

#if defined(_M_X64) || defined(__amd64__)
    mArchitecture = "x64";
#else
    mArchitecture = "x86";
#endif

    // Write architecture type.
    fprintf(r4oFile, "%s\n", mArchitecture.c_str());

    // Write list of objects and groups.
    fprintf(r4oFile, "%zu %i\n", mObjNames.size(), mNumGroups);
    
    for (int i = 0; i < mObjGroupIntervals.size(); i++)
    {
        auto & interval = mObjGroupIntervals[i];
        fprintf(r4oFile, "%s\n", mObjNames[i].c_str());
        fprintf(r4oFile, "%d %d\n", interval.first, interval.second);
    }

    // Write groups base folder.
    fprintf(r4oFile, "%s\n", mGroupsBaseFolder.c_str());
    // Write mtlb file.
    fprintf(r4oFile, "%s\n", mMtlbFilename.c_str());

    fclose(r4oFile);
    return true;
}

bool R4OFile::Load(const std::string & iFilePath)
{
    std::ifstream r4oFile(iFilePath.c_str());
    std::string line;

    if (!r4oFile.is_open())  // Couldn't open file.
        return false;

    // Read architecture type.
    std::getline(r4oFile, mArchitecture);

    // Read list of objects and groups.
    std::getline(r4oFile, line);
    std::stringstream str(line);
    int numObjs = 0, numGroups = 0;
    str >> numObjs >> numGroups;
    mNumGroups = numGroups;
    
    mObjNames.clear();
    mObjNames.reserve(numObjs);

    mObjGroupIntervals.clear();
    mObjGroupIntervals.reserve(numGroups);

    for (int i = 0; i < numObjs; i++)
    {
        // Read o name.
        std::getline(r4oFile, line);
        mObjNames.push_back(line);

        // Read o group range.
        std::getline(r4oFile, line);
        str = std::stringstream(line);
        int g0 = 0, g = 0;
        str >> g0 >> g;
        mObjGroupIntervals.push_back({g0, g});
    }

    // Read groups base folder.
    std::getline(r4oFile, line);
    mGroupsBaseFolder = line;

    // Read mtlb file.
    std::getline(r4oFile, line);
    mMtlbFilename = line;
    
    return true;
}

}  // namespace engine.
