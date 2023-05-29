#pragma once
#include "math.hpp"
#include <vector>
#include <memory>
class Node
{
public:
    example::Matrix44 transformation;
    std::vector<std::shared_ptr<Node>> children;

    Node() : transformation(1) {}
    virtual ~Node() = default;

    // void update(const Matrix44& parent_transform);

    void add_child(const std::shared_ptr<Node>& child)
    {
        children.push_back(child);
    }
};
