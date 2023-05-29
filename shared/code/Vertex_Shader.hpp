
// Copyright © 2014+ ÁRgB
// Código bajo licencia Boost Software License, version 1.0
// Ver www.boost.org/LICENSE_1_0.txt
// angel.rodriguez@esne.edu
// 2014.04

#ifndef ARGB_VERTEX_SHADER_HEADER
#define ARGB_VERTEX_SHADER_HEADER

    #include "Shader.hpp"

    namespace argb
    {

        class Vertex_Shader : public Shader
        {
        public:

            Vertex_Shader(const Source_Code & source_code)
            :
                Shader(source_code, GL_VERTEX_SHADER)
            {
            }

        };

    }

#endif
