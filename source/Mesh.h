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
    void render(GLuint model_view_matrix_id, const glm::mat4& model_view_matrix)  const;
    void load_mesh(const std::string& mesh_file_path);

private:
    GLuint vao_id;
    GLuint vbo_ids[VBO_COUNT];
    GLuint number_of_indices;
    vec3 random_color();
};
