// +--------------------------------------------+
// | File: glb_file.h                           |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-09.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

namespace engine
{

const int kORIGIN_OBJ_NAME_SIZE = 30;

// Number of attributes (format):
//         0     1   2   3
// -> 2:  pos, norm;
// -> 3:  pos, norm, uv;
// -> 4:  pos, norm, uv, tan;

struct GLB_FileHeader
{
    char _origin_name[kORIGIN_OBJ_NAME_SIZE];     // Original group filename.
    int  _num_vertices;                           // Number of vertices for VBO construction.
    int  _num_elements;                           // Number of elements for EAB construction.
    int  _vertex_format[4];                       // 3 3 2 0 (vtx, norm, uv, no tangent). 
    int  _num_attributes;                         // Number of attributes (for vertex format).
    int  _float_size;
    int  _uint_size;
    GLenum _draw_mode;                            // Is that GL_TRIANGLES or something else?
};


}  // namespace engine.