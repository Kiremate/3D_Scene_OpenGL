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
}
