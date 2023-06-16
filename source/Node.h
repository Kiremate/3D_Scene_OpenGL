#pragma once
#include "math.hpp"
#include <vector>
#include <memory>
#include <glad/glad.h>

using namespace example;
class Mesh;

class Node : public std::enable_shared_from_this<Node>
{
public:
    std::vector<std::shared_ptr<Node>> children;
    std::weak_ptr<Node> parent;

    glm::mat4 transformation;

    std::shared_ptr<Mesh> mesh; 

    Node(const glm::mat4& transformation = glm::mat4(1.0f))
        : transformation(transformation)
    {}

    void translate(const glm::vec3& offset);

    void rotate(float angle, const glm::vec3& axis);

    void scale(const glm::vec3& scaleFactor);

    void addChild(std::shared_ptr<Node> child);

    void render(GLuint model_view_matrix_id, const glm::mat4& parent_matrix);
 
};