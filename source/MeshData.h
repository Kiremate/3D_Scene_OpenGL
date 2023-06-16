#pragma once
#include <vector>
#include <Color_Buffer.hpp>
#include "math.hpp"

namespace example
{
    using argb::Rgb888;
    using argb::Color_Buffer;

    typedef Point4f Vertex;
    typedef std::vector<Vertex> Vertex_Buffer;
    typedef std::vector<int> Index_Buffer;
    typedef std::vector<Rgb888> Vertex_Colors;
    typedef std::vector< Vector3f > Vertex_Normals;
    /**
    * @class MeshData
    * @brief Contains the data structures needed for a 3D mesh.
    *
    * The MeshData class stores all the data needed for a 3D mesh, including the vertices,
    * indices, colors, and normals. This data can be transformed and displayed in a 3D scene.
    */
    class MeshData
    {
    public:
        Vertex_Buffer original_vertices;     ///< Original vertex buffer.
        Vertex_Buffer transformed_vertices;  ///< Transformed vertex buffer.
        Index_Buffer original_indices;       ///< Original index buffer.
        Vertex_Colors original_colors;       ///< Original color buffer.
        std::vector< Point4i > display_vertices; ///< Displayed vertices.
        Vertex_Normals original_normals;     ///< Original normals buffer.
        Vertex_Normals transformed_normals;  ///< Transformed normals buffer.
    };

}
