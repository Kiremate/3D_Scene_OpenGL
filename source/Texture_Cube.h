#pragma once
#include <memory>
#include <string>
#include <glad/glad.h>
#include <Color_Buffer.hpp>

namespace example
{

    class Texture_Cube
    {
    private:

        typedef argb::Color_Buffer< argb::Rgba8888 > Color_Buffer;

    private:

        GLuint texture_id;
        bool   texture_is_loaded;

    public:

        Texture_Cube(const std::string& texture_base_path);
        ~Texture_Cube();

    private:

        Texture_Cube(const Texture_Cube&) = delete;
        Texture_Cube& operator = (const Texture_Cube&) = delete;

    private:

        std::shared_ptr< Color_Buffer > load_image(const std::string& image_path);

    public:

        bool is_ok() const
        {
            return texture_is_loaded;
        }

        bool bind() const
        {
            return texture_is_loaded ? glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id), true : false;
        }

    };

}


