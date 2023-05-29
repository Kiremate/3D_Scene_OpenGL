#pragma once

#include "Node.h"
#include "Mesh.h"
#include <memory>
#include "Rasterizer.hpp"

class Scene
{
public:
    std::shared_ptr<Node> root;

    Scene();

    void render(example::Rasterizer< example::Color_Buffer<example::Rgb888> >& rasterizer, const example::Matrix44& view_projection_matrix);
    void traverse_and_render(const std::shared_ptr<Node>& node, example::Rasterizer< example::Color_Buffer<example::Rgb888> >& rasterizer, const example::Matrix44& view_projection_matrix, const example::Matrix44& parent_transform);
};
