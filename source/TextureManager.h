#pragma once
#include <string>
#include <map>

struct TextureData {
    unsigned int openGlTextureId;
};

class TextureManager {
public:
    TextureManager();
    ~TextureManager();

    bool loadTexture(const std::string& id, const std::string& filepath);
    unsigned int getTexture(const std::string& id);

private:
    std::map<std::string, TextureData> textures;
};
