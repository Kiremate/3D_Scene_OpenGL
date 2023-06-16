#include "Node.h"
#include "Mesh.h"

using namespace example;

void Node::render()
{
    if (mesh)
        mesh->render();

    for (auto& child : children)
        child->render();
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
