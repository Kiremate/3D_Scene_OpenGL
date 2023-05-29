#include "TextureManager.h"
#include <iostream>
#include <cassert>
#include <SOIL2.h>
#include <glad/glad.h>
#include <glm/glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>                 // value_ptr
#include <fstream>  

TextureManager::TextureManager() {
}

TextureManager::~TextureManager() {
    for (auto& item : textures) {
        // Delete the OpenGL texture
        glDeleteTextures(1, &item.second.openGlTextureId);
    }
}


bool TextureManager::loadTexture(const std::string& id, const std::string& filepath) {
    // First, check if the file exists
    std::ifstream f(filepath.c_str());
    if (!f.good()) {
        throw std::runtime_error("File not found: " + filepath);
    }
    f.close();

    GLuint texture_id;
    glGenTextures(1, &texture_id);

    int width = 0;
    int height = 0;
    int imageChannels = 0;

    uint8_t* data = SOIL_load_image(filepath.c_str(), &width, &height, &imageChannels, SOIL_LOAD_RGBA);

    if (data == NULL) {
        std::cerr << "Texture loading failed for: " << filepath << std::endl;
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    TextureData textureData = { texture_id };
    auto inserted = textures.insert(std::make_pair(id, textureData));
    return inserted.second;
}

GLuint TextureManager::getTexture(const std::string& id) {
    auto it = textures.find(id);
    if (it != textures.end()) {
        return it->second.openGlTextureId;
    }
    else {
        // Return a default texture ID, or throw an exception.
        return 0;  // Or throw an exception
    }
}
