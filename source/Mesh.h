#pragma once
#include "Node.h"
#include "MeshData.h"
#include <string>
#include "Camera.h"
class Mesh : public Node
{
public:
    example::Vertex_Buffer original_vertices;
    example::Vertex_Buffer transformed_vertices;
    example::UV_Buffer original_uvs;   // Add this
    example::Index_Buffer original_indices;
    example::Vertex_Colors original_colors;
    Mesh(const std::string& file_path);
    void transform_vertices(Camera& camera, float width, float height);
};
