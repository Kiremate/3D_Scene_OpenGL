#include "View.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>                 // value_ptr
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
using namespace std;
using namespace glm;

namespace example
{

    const string View::vertex_shader_code =

        "#version 330\n"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec3 vertex_color;"
        ""
        "out vec3 front_color;"
        ""
        "void main()"
        "{"
        "   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
        "   front_color = vertex_color;"
        "}";

    const string View::fragment_shader_code =

        "#version 330\n"
        ""
        "in  vec3    front_color;"
        "out vec4 fragment_color;"
        ""
        "void main()"
        "{"
        "    fragment_color = vec4(front_color, 0.5);"
        "}";

    const string View::postprocess_vertex_shader_code =
        "#version 330\n"
        "in vec2 position;"
        "in vec2 texCoords;"
        "out vec2 TexCoords;"
        "void main()"
        "{"
        "   gl_Position = vec4(position, 0.0f, 1.0f);"
        "   TexCoords = texCoords;"
        "}";

    const string View::postprocess_fragment_shader_code =
        "#version 330\n"
        "in vec2 TexCoords;"
        "out vec4 color;"
        "uniform sampler2D screenTexture;"
        "void main()"
        "{"
        "   vec3 texColor = texture(screenTexture, TexCoords).rgb;"
        "   float gray = dot(texColor, vec3(0.3, 0.59, 0.11));"
        "   color = vec4(vec3(gray), 1.0f);"
        "}";

    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };



    View::View(int width, int height)
        :
        skybox("../../shared/assets/sky-cube-map-"),
        frameBuffer(width, height),
        angle(0)
    {
        // Bind frame buffer
        frameBuffer.Bind();


        glm::mat4 transformation = glm::mat4(1.f);
        transformation = glm::translate(transformation, glm::vec3(0.f, 0.f, -3.f));
        transformation = glm::rotate(transformation, angle, glm::vec3(0.f, 1.f, 0.f));
        rootNode = std::make_shared<Node>(transformation);
        std::shared_ptr<Mesh> bunnyMesh = std::make_shared<Mesh>("../../shared/assets/stanford-bunny.obj");
        rootNode->mesh = bunnyMesh;

        // Se establece la configuración básica:
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glClearColor(.1f, .1f, .1f, 1.f);
        textureManager.loadTexture("texture1", "../../shared/assets/textures/baju_Text.jpg");
        textureManager.loadTexture("texture2", "../../shared/assets/textures/Hair_texture.png");
        textureManager.loadTexture("texture3", "../../shared/assets/textures/Skin.png");
       
        // quad vertices for post-processing
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        // compile the postprocessing shaders
        postprocess_program_id = compile_postprocessing_shaders();
        // Se compilan y se activan los shaders:

        program_id = compile_shaders();

        glUseProgram(program_id);

        model_view_matrix_id = glGetUniformLocation(program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation(program_id, "projection_matrix");
        // We unbind the FrameBuffer
        frameBuffer.Unbind();
        // Post-processing...
        glBindTexture(GL_TEXTURE_2D, frameBuffer.GetTextureBuffer());
        resize(width, height);
        angle_around_x = angle_delta_x = 0.0;
        angle_around_y = angle_delta_y = 0.0;
        pointer_pressed = false;
    }

    View::~View()
    {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }
    void View::update()
    {
        angle += 0.01f;
        angle_around_x += angle_delta_x;
        angle_around_y += angle_delta_y;

        glm::mat4 camera_rotation(1);

        // Add the rotations due to mouse dragging
        camera_rotation = glm::rotate(camera_rotation, angle_around_y, glm::vec3(0.f, 1.f, 0.f));
        camera_rotation = glm::rotate(camera_rotation, angle_around_x, glm::vec3(1.f, 0.f, 0.f));

        camera.set_target(0, 0, -1);
        camera.rotate(camera_rotation);
    }

    void View::render()
    {
        // bind to framebuffer and draw scene as we normally would to color texture
        frameBuffer.Bind(); // Use FrameBuffer's Bind method instead of manual binding
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // First, render the skybox
        skybox.render(camera);

        // Now switch to the shader program for the bunny
        glUseProgram(program_id);

        // Send the projection matrix to the shader
        glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(camera.get_projection_matrix()));

        // Model transformation for the bunny
        glm::mat4 model_matrix = glm::mat4(1.f);
        model_matrix = glm::translate(model_matrix, glm::vec3(0.f, 0.f, -3.f));
        model_matrix = glm::rotate(model_matrix, angle, glm::vec3(0.f, 1.f, 0.f));

        // Combine the model and view transformations
        glm::mat4 model_view_matrix = camera.get_model_view_matrix() * model_matrix;
        frameBuffer.Unbind(); // Use FrameBuffer's Unbind method instead of manual unbinding

        // Send the transformation to the shader
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));

        // Render the root node (which will recursively render all children)
        rootNode->render(model_view_matrix_id, glm::mat4(1.0f));

        frameBuffer.Unbind(); // Use FrameBuffer's Unbind method instead of manual unbinding

        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // use the postprocessing shader
        glUseProgram(postprocess_program_id);
        glBindTexture(GL_TEXTURE_2D, frameBuffer.GetTextureBuffer()); // use the FrameBuffer's texture instead of the manually created one
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void View::resize(int new_width, int new_height)
    {
        width = new_width;
        height = new_height;

        camera.set_ratio(float(width) / height);

        glViewport(0, 0, width, height);
    }


    GLuint View::compile_shaders()
    {
        GLint succeeded = GL_FALSE;

        // Se crean objetos para los shaders:

        GLuint   vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

        // Se carga el código de los shaders:

        const char* vertex_shaders_code[] = { vertex_shader_code.c_str() };
        const char* fragment_shaders_code[] = { fragment_shader_code.c_str() };
        const GLint    vertex_shaders_size[] = { (GLint)vertex_shader_code.size() };
        const GLint  fragment_shaders_size[] = { (GLint)fragment_shader_code.size() };

        glShaderSource(vertex_shader_id, 1, vertex_shaders_code, vertex_shaders_size);
        glShaderSource(fragment_shader_id, 1, fragment_shaders_code, fragment_shaders_size);

        // Se compilan los shaders:

        glCompileShader(vertex_shader_id);
        glCompileShader(fragment_shader_id);

        // Se comprueba que si la compilación ha tenido éxito:

        glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(vertex_shader_id);

        glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(fragment_shader_id);

        // Se crea un objeto para un programa:

        GLuint program_id = glCreateProgram();

        // Se cargan los shaders compilados en el programa:

        glAttachShader(program_id, vertex_shader_id);
        glAttachShader(program_id, fragment_shader_id);

        // Se linkan los shaders:

        glLinkProgram(program_id);

        // Se comprueba si el linkage ha tenido éxito:

        glGetProgramiv(program_id, GL_LINK_STATUS, &succeeded);
        if (!succeeded) show_linkage_error(program_id);

        // Se liberan los shaders compilados una vez se han linkado:

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);

        return (program_id);
    }


    void View::show_compilation_error(GLuint shader_id)
    {
        string info_log;
        GLint  info_log_length;

        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

        info_log.resize(info_log_length);

        glGetShaderInfoLog(shader_id, info_log_length, NULL, &info_log.front());

        cerr << info_log.c_str() << endl;

#ifdef _MSC_VER
        //OutputDebugStringA (info_log.c_str ());
#endif

        assert(false);
    }

    void View::show_linkage_error(GLuint program_id)
    {
        string info_log;
        GLint  info_log_length;

        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);

        info_log.resize(info_log_length);

        glGetProgramInfoLog(program_id, info_log_length, NULL, &info_log.front());

        cerr << info_log.c_str() << endl;

#ifdef _MSC_VER
        //OutputDebugStringA (info_log.c_str ());
#endif

        assert(false);
    }

    vec3 View::random_color()
    {
        return vec3
        (
            float(rand()) / float(RAND_MAX),
            float(rand()) / float(RAND_MAX),
            float(rand()) / float(RAND_MAX)
        );
    }

    void View::on_key(int key_code)
    {
    }

    void View::on_drag(int pointer_x, int pointer_y)
    {
        if (pointer_pressed)
        {
            angle_delta_x = 0.025f * float(last_pointer_y - pointer_y) / float(height);
            angle_delta_y = 0.025f * float(last_pointer_x - pointer_x) / float(width);
        }
    }

    void View::on_click(int pointer_x, int pointer_y, bool down)
    {
        if ((pointer_pressed = down) == true)
        {
            last_pointer_x = pointer_x;
            last_pointer_y = pointer_y;
        }
        else
        {
            angle_delta_x = angle_delta_y = 0.0;
        }
    }


    GLuint View::compile_postprocessing_shaders()
    {
        GLint succeeded = GL_FALSE;

        // Create objects for the shaders:

        GLuint   vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

        // Load the code of the shaders:

        const char* vertex_shaders_code[] = { postprocess_vertex_shader_code.c_str() };
        const char* fragment_shaders_code[] = { postprocess_fragment_shader_code.c_str() };
        const GLint    vertex_shaders_size[] = { (GLint)postprocess_vertex_shader_code.size() };
        const GLint  fragment_shaders_size[] = { (GLint)postprocess_fragment_shader_code.size() };

        glShaderSource(vertex_shader_id, 1, vertex_shaders_code, vertex_shaders_size);
        glShaderSource(fragment_shader_id, 1, fragment_shaders_code, fragment_shaders_size);

        // Compile the shaders:

        glCompileShader(vertex_shader_id);
        glCompileShader(fragment_shader_id);

        // Check if the compilation was successful:

        glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(vertex_shader_id);

        glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(fragment_shader_id);

        // Create an object for a program:

        GLuint program_id = glCreateProgram();

        // Load the compiled shaders into the program:

        glAttachShader(program_id, vertex_shader_id);
        glAttachShader(program_id, fragment_shader_id);

        // Link the shaders:

        glLinkProgram(program_id);

        // Check if the linkage was successful:

        glGetProgramiv(program_id, GL_LINK_STATUS, &succeeded);
        if (!succeeded) show_linkage_error(program_id);

        // Free the compiled shaders once they have been linked:

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);

        return (program_id);
    }
}
