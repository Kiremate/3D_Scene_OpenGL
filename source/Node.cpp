#include "Node.h"
#include "Mesh.h"

using namespace example;

void Node::render(GLuint model_view_matrix_id, const glm::mat4& parent_matrix) {
    // Calculate the model-view matrix for this Node by applying this Node's transformation
    // to the parent's transformation matrix.
    glm::mat4 model_view_matrix = parent_matrix * transformation;

    if (mesh)
        mesh->render(model_view_matrix_id, model_view_matrix);

    // Pass the model-view matrix to the children so they can apply their transformations
    for (auto& child : children)
        child->render(model_view_matrix_id, model_view_matrix);
}

void Node::addChild(std::shared_ptr<Node> child)
{
    child->parent = shared_from_this();
    children.push_back(child);
}

void Node::scale(const glm::vec3& scaleFactor)
{
    transformation = glm::scale(transformation, scaleFactor);
}

void Node::rotate(float angle, const glm::vec3& axis) 
{
    transformation = glm::rotate(transformation, glm::radians(angle), axis);
}

void Node::translate(const glm::vec3& offset)
{
    transformation = glm::translate(transformation, offset);
}
