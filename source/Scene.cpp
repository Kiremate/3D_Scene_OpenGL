#include "Scene.h"

Scene::Scene()
{
    root = std::make_shared<Node>();
}

void Scene::render(example::Rasterizer< example::Color_Buffer<example::Rgb888> >& rasterizer, const example::Matrix44& view_projection_matrix)
{
    traverse_and_render(root, rasterizer, view_projection_matrix, example::Matrix44(1));
}

void Scene::traverse_and_render(const std::shared_ptr<Node>& node, example::Rasterizer< example::Color_Buffer<example::Rgb888> >& rasterizer, const example::Matrix44& view_projection_matrix, const example::Matrix44& parent_transform)
{
    example::Matrix44 current_transform = parent_transform * node->transformation;

    if (std::dynamic_pointer_cast<Mesh>(node))
    {
        std::shared_ptr<Mesh> mesh_node = std::dynamic_pointer_cast<Mesh>(node);
        // Render the mesh_node using the rasterizer and the current_transform * view_projection_matrix
    }

    for (auto& child : node->children)
    {
        traverse_and_render(child, rasterizer, view_projection_matrix, current_transform);
    }
}
