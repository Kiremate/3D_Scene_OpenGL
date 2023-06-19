#include <cassert>
#include <iostream>
#include <glad/glad.h>
#include "Skybox.h"
#include "Vertex_Shader.hpp"
#include "Fragment_Shader.hpp"
#include "Camera.h"
#include "ShaderUtility.h"

namespace example
{

    using namespace std;
    using namespace glm;

    const GLfloat Skybox::coordinates[] =
    {
        -1.0f, +1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        +1.0f, -1.0f, -1.0f,
        +1.0f, -1.0f, -1.0f,
        +1.0f, +1.0f, -1.0f,
        -1.0f, +1.0f, -1.0f,
        -1.0f, -1.0f, +1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, +1.0f, -1.0f,
        -1.0f, +1.0f, -1.0f,
        -1.0f, +1.0f, +1.0f,
        -1.0f, -1.0f, +1.0f,
        +1.0f, -1.0f, -1.0f,
        +1.0f, -1.0f, +1.0f,
        +1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, -1.0f,
        +1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, +1.0f,
        -1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, +1.0f,
        +1.0f, -1.0f, +1.0f,
        -1.0f, -1.0f, +1.0f,
        -1.0f, +1.0f, -1.0f,
        +1.0f, +1.0f, -1.0f,
        +1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, +1.0f,
        -1.0f, +1.0f, +1.0f,
        -1.0f, +1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, +1.0f,
        +1.0f, -1.0f, -1.0f,
        +1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, +1.0f,
        +1.0f, -1.0f, +1.0f,
    };

    const std::string Skybox::vertex_shader_code =

        "#version 330\n"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        ""
        "out vec3 texture_coordinates;"
        ""
        "void main()"
        "{"
        "   texture_coordinates = vec3(vertex_coordinates.x, -vertex_coordinates.y, vertex_coordinates.z);"
        "   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
        "}";

    const std::string Skybox::fragment_shader_code =

        "#version 330\n"
        ""
        "in  vec3 texture_coordinates;"
        "out vec4 fragment_color;"
        ""
        "uniform samplerCube texture_cube;"
        ""
        "void main()"
        "{"
        "    fragment_color = texture (texture_cube, texture_coordinates);"
        "}";

    Skybox::Skybox(const std::string& texture_base_path)
        :
        texture_cube(texture_base_path)
    {
        assert(texture_cube.is_ok());

        // Se compilan y linkan los shaders:

        shader_program_id = ShaderUtility::CompileShaders(vertex_shader_code, fragment_shader_code);

        model_view_matrix_id = glGetUniformLocation(shader_program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation(shader_program_id, "projection_matrix");

        // Se generan índices para los VBOs del cubo:

        glGenBuffers(1, &vbo_id);
        glGenVertexArrays(1, &vao_id);

        // Se activa el VAO del cubo para configurarlo:

        glBindVertexArray(vao_id);

        // Se suben a un VBO los datos de coordenadas y se vinculan al VAO:

        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindVertexArray(0);
    }

    Skybox::~Skybox()
    {
        // Se libera el VBO y el VAO usados:
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(1, &vbo_id);
    }

    void Skybox::render(const Camera& camera)
    {
        glUseProgram(shader_program_id);
        glDepthMask(GL_FALSE);

        texture_cube.bind();

        const glm::mat4   model_view_matrix = camera.get_transform_matrix_inverse();
        const glm::mat4& projection_matrix = camera.get_projection_matrix();

        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
        glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        glDepthMask(GL_FALSE);

        glBindVertexArray(vao_id);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDepthMask(GL_TRUE);

        glBindVertexArray(0);
        glUseProgram(0);
        glDepthMask(GL_TRUE); // enable writing into the depth buffer again.
    }
}
