#include "obj_parser.h"


#include <vector>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <algorithm>

namespace obj_tool
{
void PrintStringList(const std::vector<std::string> & stringList)
{
    std::cout << "{";
    for (size_t i = 0; i < stringList.size(); i++)
    {
        std::cout << "\"" << stringList[i] << "\"";

        if (i < stringList.size()-1)
            std::cout << ", ";
    }
    std::cout << "}";
}

void ObjParser::SplitByString(const std::string & input, const std::string & separator, 
    std::vector<std::string> & stringList, bool removeEmptyComponents)
{
    size_t start  = 0;
    size_t length = separator.size(); 

    while (start < input.size())
    {
        size_t pos = input.find(separator, start);
        stringList.push_back(input.substr(start, pos-start));
        if (pos == std::string::npos)
        {
            break;
        }
        start = pos + length;
    }

    if (removeEmptyComponents)
        stringList.erase( std::remove( stringList.begin(), stringList.end(), ""), stringList.end() ); 
}

void ObjParser::PreprocessLine(const std::string & rawLine, std::string & filteredLine)
{
    std::string noRepeatedSpaces;
    noRepeatedSpaces.reserve(rawLine.size());

    // Remove repeated spaces.
    for (size_t i = 0; i < rawLine.size(); i++)
    {
        if ( (!isspace(rawLine[i]))                        // If not space,
            || ((i > 0) && (rawLine[i] != rawLine[i-1]))   // or a not repeated space
            || (i == 0))                                   // or space at the first position.
        {
            noRepeatedSpaces.push_back(rawLine[i]);
        }
    }

    // Remove first space at the beginning.
    if (noRepeatedSpaces.size() > 0 && isspace(noRepeatedSpaces[0]))
    {
        filteredLine = noRepeatedSpaces.substr(1);
    }
    else
    {
        filteredLine = noRepeatedSpaces;
    }
}

bool ObjParser::ParseVec2f(const char * line, QVector2D & attrib, 
    bool verbose)
{
    if (sscanf(line, "%f %f", &attrib[0], &attrib[1]) != 2)
    {
        if (verbose)
            std::cerr << "ERROR The attribute must have 2 values. " << std::endl;
        return false;
    }
    else
    {
        return true;
    }
}

bool ObjParser::ParseVec3f(const char * line, QVector3D & attrib, 
    bool verbose)
{

    if (sscanf(line, "%f %f %f", &attrib[0], &attrib[1], &attrib[2]) != 3)
    {
        if (verbose)
            std::cerr << "ERROR The attribute must have 3 values. " << std::endl;
        return false;
    }
    else
    {
        return true;
    }
}

bool ObjParser::ParseFace(const std::string & lineData, Face & face, bool verbose)
{
    // Check if there is any data.
    if (lineData.size() == 0) {
        if (verbose)
            std::cerr << "ERROR Empty line." << std::endl;
        return false;
    }

    int i0 = 0;
    int vtxNumber = 0;
    for (int i = i0; i < lineData.size(); )
    {
        // Search for the next space index. "
        i++;
        for (; i < lineData.size() && !isspace(lineData[i]); ++i); 

        // Read the i-th vertex data into components.
        std::string vertexInfo = lineData.substr(i0, i-i0);
        std::vector<std::string> subcomponents;
        ObjParser::SplitByString(vertexInfo, "/", subcomponents, false);

        i0 = i+1;

        // Skip empty component.
        if (vertexInfo.size() == 0)
            continue;

        // Check the number of subcomponents.
        if (subcomponents.size() < 1 || subcomponents.size() > 3)
        {
            if (verbose)
                std::cerr << "ERROR Wrong vertex indices format. " << std::endl;
            return false;
        }

        if (face.GetNumVertices() >= 4) 
        {
            if (verbose)
                std::cerr << "ERROR A face (polygon) must contain at least 3, and at most 4 vertices." << std::endl;
            //return false;
            return true;
        }
        face.PushVertex();

        // Convert subcomponents into indices.
        for (int j = 0; j < subcomponents.size(); j++)
        {
            char* p;
            int coord = strtol(subcomponents[j].c_str(), &p, 10);
            if (*p)
            {
                if (verbose)
                    std::cerr << "ERROR Could not read index: '" << subcomponents[j] << "' " << std::endl;
                return false;
            }

            // Index is provided.
            if (subcomponents[j].size() > 0)
            {
                face[vtxNumber][j] = coord-1;
            }
        }

        vtxNumber++;
    }

    if (vtxNumber < 3) 
    {
        if (verbose)
            std::cerr << "ERROR A face (polygon) must contain at least 3 vertices." << std::endl;
        return false;
    }

    return true;
}

bool ObjParser::LoadMTL(const std::string & filename, std::vector<ObjMaterial> & mtllib)
{
    std::ifstream objFile(filename);
    std::string line;
    int lineNumber = 0;

    ObjMaterial currentMaterial;
    bool noMaterialsSoFar = true;

    if (!objFile.is_open())
    {
        std::cerr << "ERROR Could not open .mtl file at '" << filename << "'. " << std::endl;
        return false;
    }

    while (std::getline(objFile, line))
    {
        lineNumber++;

        // Filter line and split into components (also, remove empty components).
        std::string filteredLine;
        ObjParser::PreprocessLine(line, filteredLine);

        if (filteredLine.size() < 1)
            continue;

        // Find first white space.
        int ws_pos = 0;
        for (int i = 0; i < filteredLine.size() && !isspace(filteredLine[i]); i++)
            ws_pos++;

        // Split line into two pieces: the ID and its data.
        std::string lineID = filteredLine.substr(0, ws_pos - 0);
        std::string lineData;

        // If first white-space is at the end, then the line data is empty.
        if (ws_pos != filteredLine.size())
            lineData = filteredLine.substr(ws_pos + 1);

        // Check first component type.
        if (lineID == atomic::NEWMTL)
        {
            if (!noMaterialsSoFar)
            {
                mtllib.push_back(currentMaterial);
            }

            // Reset current material.
            currentMaterial = ObjMaterial();
            currentMaterial.mName = (lineData == "") ? "??" : lineData;

            noMaterialsSoFar = false;
        }
        else if (lineID == atomic::KA)
        {
            QVector3D ambient;
            if (!ObjParser::ParseVec3f(lineData.c_str(), ambient, true)) 
            {
                std::cerr << "\t^ at line " << lineNumber << " ('" << filename << "')" << std::endl;
                std::cerr << lineNumber << ": '" << line << "'\n";  
                return false;
            }
            currentMaterial.mKa = ambient;
        }
        else if (lineID == atomic::KD)
        {
            QVector3D diffuse;
            if (!ObjParser::ParseVec3f(lineData.c_str(), diffuse, true)) 
            {
                std::cerr << "\t^ at line " << lineNumber << " ('" << filename << "')" << std::endl;
                std::cerr << lineNumber << ": '" << line << "'\n";  
                return false;
            }
            currentMaterial.mKd = diffuse;
        }
        else if (lineID == atomic::KS)
        {
            QVector3D specular;
            if (!ObjParser::ParseVec3f(lineData.c_str(), specular, true)) 
            {
                std::cerr << "\t^ at line " << lineNumber << " ('" << filename << "')" << std::endl;
                std::cerr << lineNumber << ": '" << line << "'\n";  
                return false;
            }
            currentMaterial.mKs = specular;
        }
        else if (lineID == atomic::ILLUM)
        {
            sscanf(lineData.c_str(), "%d", &currentMaterial.mIllum);
        }
        else if (lineID == atomic::D)
        {
            sscanf(lineData.c_str(), "%f", &currentMaterial.mD);
        }
        else if (lineID == atomic::NS)
        {
            sscanf(lineData.c_str(), "%f", &currentMaterial.mNs);
        }
        else if (lineID == atomic::MAP_KS)
        {
            currentMaterial.mMapKs = lineData;
            // TODO: Check if file really exists.
        }
        else if (lineID == atomic::MAP_KD)
        {
            currentMaterial.mMapKd = lineData;
            // TODO: Check if file really exists.
        }
        else if (lineID == atomic::MAP_BUMP)
        {
            currentMaterial.mMapBump = lineData;
        }
        else if (lineID == atomic::BUMP)
        {
            currentMaterial.mMapBump = lineData;
        }
        // Ignore other types of line.
    }

    mtllib.push_back(currentMaterial);
}

bool ObjParser::LoadObj(const std::string & path, std::string & objname, 
                        ObjData & objMesh, bool verbose)
{
    std::string filename = path + "/" + objname;
    std::ifstream objFile(filename);
    std::string line;
    int lineNumber = 0;
    bool smooth = false;

    if (!objFile.is_open())
    {
        if (verbose)
            std::cerr << "ERROR Could not open .obj file at '" << filename << "'. " << std::endl;
        return false;
    }

    objMesh.SetObjFilename(objname);

    while (std::getline(objFile, line))
    {
        lineNumber++;

        // Filter line and split into components (also, remove empty components).
        std::string filteredLine;
        ObjParser::PreprocessLine(line, filteredLine);

        if (filteredLine.size() < 1)
            continue;

        // Find first white space.
        int ws_pos = 0;
        for (int i = 0; i < filteredLine.size() && !isspace(filteredLine[i]); i++)
            ws_pos++;

        // Split line into two pieces: the ID and its data.
        std::string lineID   = filteredLine.substr(0, ws_pos-0);
        std::string lineData;
        
        // If first white-space is at the end, then the line data is empty.
        if (ws_pos != filteredLine.size())
            lineData = filteredLine.substr(ws_pos + 1);

        // Check first component type.
        if (lineID == atomic::G)  // New group.
        {
            objMesh.AddGroup(lineData);
        }
        else if (lineID == atomic::O)  // New object.
        {
            objMesh.AddObj(lineData);
        }
        else if (lineID == atomic::S)
        {
            smooth = (lineData == "off") ? false : true;
        }
        else
        {
            if (lineID == atomic::V)  // New vertex.
            {
                QVector3D vertex;
                if (!ObjParser::ParseVec3f(lineData.c_str(), vertex, verbose)) 
                {
                    if (verbose)
                    {
                        std::cerr << "\t^ at line " << lineNumber << " ('" << filename << "')" << std::endl;
                        std::cerr << lineNumber << ": '" << line << "'\n";  
                    }
                    return false;
                }

                objMesh.AddVertex(vertex);
            }
            else if (lineID == atomic::VT)  // New texture uv.
            {
                QVector2D uv;
                if (!ObjParser::ParseVec2f(lineData.c_str(), uv, verbose)) 
                {
                    if (verbose)
                    {
                        std::cerr << "\t^ at line " << lineNumber << " ('" << filename << "')" << std::endl;
                        std::cerr << lineNumber << ": '" << line << "'\n";  
                    }
                    return false;
                }

                objMesh.AddUV(uv);
            }
            else if (lineID == atomic::VN)  // New normal vector.
            {
                QVector3D normal;
                if (!ObjParser::ParseVec3f(lineData.c_str(), normal, verbose)) 
                {
                    if (verbose)
                    {
                        std::cerr << "\t^ at line " << lineNumber << " ('" << filename << "')" << std::endl;
                        std::cerr << lineNumber << ": '" << line << "'\n";  
                    }
                    return false;
                }

                objMesh.AddNormal(normal);
            }
            else if (lineID == atomic::F)  // New face.
            {
                Face face;
                if (!ObjParser::ParseFace(lineData, face, verbose))
                {
                    if (verbose)
                    {
                        std::cerr << "\t^ at line " << lineNumber << " ('" << filename << "')" << std::endl;
                        std::cerr << lineNumber << ": '" << line << "'\n";  
                    }
                    return false;
                }

                face.SetSmoothShading(smooth);
                objMesh.AddFace(face);
            }
            else if (lineID == atomic::MTLLIB)
            {
                std::string mtllibFilename = path + "/" + lineData;

                // Load material library.
                if (!this->LoadMTL(mtllibFilename, objMesh.GetMaterialLib()))
                {
                    if (verbose)
                    {
                        std::cerr << "ERROR Couldn't load mtllib." << std::endl;
                        std::cerr << "\t^ at line " << lineNumber << " ('" << filename << "')" << std::endl;
                        std::cerr << lineNumber << ": '" << line << "'\n";  
                    }
                    return false;
                }
                objMesh.SetMaterialFilename(lineData);
            }
            else if (lineID == atomic::USEMTL)
            {
                const std::vector<ObjMaterial> & materialLib = objMesh.GetMaterialLib();

                if (lineData == "")
                {
                    if (verbose)
                    {
                        std::cerr << "ERROR Empty material name." << std::endl;
                        std::cerr << "\t^ at line " << lineNumber << " ('" << filename << "')" << std::endl;
                        std::cerr << lineNumber << ": '" << line << "'\n";  
                    }
                    return false;
                }

                // Search for material with the same name in the library.
                int index = -1;
                for (int i = 0; i < materialLib.size(); i++)
                {
                    if (lineData == materialLib[i].mName)  // Found.
                        index = i;
                }

                if (index == -1)
                {
                    if (verbose)
                    {
                        std::cerr << "ERROR Material not found in mtllib." << std::endl;
                        std::cerr << "\t^ at line " << lineNumber << " ('" << filename << "')" << std::endl;
                        std::cerr << lineNumber << ": '" << line << "'\n";  
                    }
                    return false;
                }

                objMesh.SetMaterialIndexForCurrentGroup(index);
            }

            // Ignore comments and other kinds of line.
        }
    }

    return true;
}

}  // namespace obj_tool.