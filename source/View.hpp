
// Este código es de dominio público.
// angel.rodriguez@esne.edu
// 2014.05

#ifndef VIEW_HEADER
#define VIEW_HEADER

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include "Camera.h"
#include "Skybox.h"

namespace example
{

    using glm::vec3;

    class View
    {
    private:
        Camera camera;
        Skybox skybox;

        int    width;
        int    height;

        float  angle_around_x;
        float  angle_around_y;
        float  angle_delta_x;
        float  angle_delta_y;

        bool   pointer_pressed;
        int    last_pointer_x;
        int    last_pointer_y;

        enum
        {
            COORDINATES_VBO,
            COLORS_VBO,
            INDICES_EBO,
            VBO_COUNT
        };

        static const std::string   vertex_shader_code;
        static const std::string fragment_shader_code;

        GLuint  vbo_ids[VBO_COUNT];
        GLuint  vao_id;

        GLsizei number_of_indices;

        GLint   model_view_matrix_id;
        GLint   projection_matrix_id;

        float   angle;

    public:

        View(int width, int height);
        ~View();

        void   update();
        void   render();
        void   resize(int width, int height);
        void on_key(int key_code);
        void on_drag(int pointer_x, int pointer_y);
        void on_click(int pointer_x, int pointer_y, bool down);

    private:

        GLuint compile_shaders();
        void   show_compilation_error(GLuint  shader_id);
        void   show_linkage_error(GLuint program_id);
        void   load_mesh(const std::string& mesh_file_path);
        vec3   random_color();

    };

}

#endif
