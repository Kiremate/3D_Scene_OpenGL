#pragma once
#include "Node.h"
#include "MeshData.h"
#include <string>
#include "Camera.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "MeshDataTypes.h"

using namespace glm;
class Mesh 
{
public:
    Mesh(const std::string& filePath);
    ~Mesh();
    void render() const;
    void render(GLuint textureId) const;
    void load_mesh(const std::string& mesh_file_path);
    // Getter
    GLuint getVaoId() const { return vao_id; } 
    void setName(const std::string& name) { meshName = name; }
    std::string getName() const { return meshName; }
private:
    std::string meshName;
    GLuint vao_id;
    GLuint vbo_ids[VBO_COUNT];
    GLuint number_of_indices;
    vec3 random_color();
};
