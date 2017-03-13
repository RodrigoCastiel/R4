// +--------------------------------------------+
// | File: obj_parser.h                         |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-09.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <QVector2D>
#include <QVector3D>

#include "obj_data.h"
#include "obj_material.h"

namespace obj_tool
{

namespace atomic 
{
    const std::string COMMENT = "#";  // Comment line marker.
    const std::string V       = "v";  // Vertex.
    const std::string VT      = "vt"; // Vertex texture.
    const std::string VN      = "vn"; // Vertex normal.
    const std::string F       = "f";  // Face.
    const std::string G       = "g";  // Group.
    const std::string O       = "o";  // Object.
    const std::string EMPTY = "";
    const std::string SPACE = " ";
    const std::string USEMTL = "usemtl";
    const std::string MTLLIB = "mtllib";
    const std::string S      = "s";

    const std::string NEWMTL = "newmtl";
    const std::string ILLUM = "illum";
    const std::string D = "d";
    const std::string NS = "Ns";
    const std::string KA = "Ka";
    const std::string KS = "Ks";
    const std::string KD = "Kd";
    const std::string MAP_KS = "map_Ks";
    const std::string MAP_KD = "map_Kd";
    const std::string MAP_BUMP = "map_Bump";
    const std::string BUMP = "bump";

}  // namespace atomic.

class ObjParser
{
public:
    // Splits the input string into a list of strings according to a separator string.
    void SplitByString(const std::string & input, const std::string & separator, 
        std::vector<std::string> & stringList, bool removeEmptyComponents);

    // Filters repeated spaces.
    void PreprocessLine(const std::string & rawLine, std::string & outputLine);

    // Reads the line and converts them into attribute data.
    // Returns false if the line is invalid. 
    // Otherwise, it returns true and stores the data into attrib.
    bool ParseVec2f(const char * line, QVector2D & attrib, bool verbose = false);
    bool ParseVec3f(const char * line, QVector3D & attrib, bool verbose = false);

    // Reads the line component and converts them into face data (list of vertices, normal and uv).
    // Returns false if the line is invalid. 
    // Otherwise, it returns true and stores the data into face.
    bool ParseFace(const std::string & lineData, Face & face, bool verbose = false);

    bool LoadMTL(const std::string & filename, std::vector<ObjMaterial> & mtllib);

    // TODO: brief description.
    bool LoadObj(const std::string & path, std::string & objname, ObjData & objMesh, bool verbose);
};

}  // namespace obj_tool.