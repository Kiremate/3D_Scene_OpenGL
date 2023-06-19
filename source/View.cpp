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
#include "ShaderUtility.h"
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


    View::View(int width, int height)
        :
        skybox("../../shared/assets/sky-cube-map-"),
        frameBuffer(width, height),
        angle(0),
        postProcess(frameBuffer)
    {
        postProcess.Init(); // call Init function to set up VAO and VBO
        glm::mat4 transformation = glm::mat4(1.f);
        transformation = glm::translate(transformation, glm::vec3(0.f, 0.f, -3.f));
        transformation = glm::rotate(transformation, angle, glm::vec3(0.f, 1.f, 0.f));
        rootNode = std::make_shared<Node>(transformation);
        std::shared_ptr<Mesh> bunnyMesh = std::make_shared<Mesh>("../../shared/assets/stanford-bunny.obj");
        rootNode->mesh = bunnyMesh;

        // Se establece la configuración básica:
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glClearColor(.1f, .1f, .1f, 1.f);
        textureManager.loadTexture("texture1", "../../shared/assets/textures/baju_Text.jpg");
        textureManager.loadTexture("texture2", "../../shared/assets/textures/Hair_texture.png");
        textureManager.loadTexture("texture3", "../../shared/assets/textures/Skin.png");

        program_id = ShaderUtility::CompileShaders(vertex_shader_code, fragment_shader_code);
        model_view_matrix_id = glGetUniformLocation(program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation(program_id, "projection_matrix");

        resize(width, height);
        angle_around_x = angle_delta_x = 0.0;
        angle_around_y = angle_delta_y = 0.0;
        pointer_pressed = false;
    }

    View::~View()
    {
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
        // Render to the framebuffer
        frameBuffer.Bind();
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the skybox here
        skybox.render(camera);

        // Render opaque objects
        glDisable(GL_BLEND);
        rootNode->render(model_view_matrix_id, glm::mat4(1.0f));

        // Done rendering to the framebuffer
        frameBuffer.Unbind();

        // Now we apply post-processing on the rendered scene (which is on our framebuffer texture)
        // and draw the result to the default framebuffer

        // Clear the default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use post-processing shader
        glUseProgram(postProcess.getProgramId());

        // Activate texture unit and bind the texture we want to post-process
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, frameBuffer.GetTextureBuffer());

        // Get uniform location for "screenTexture" and set it to 0
        GLint texLoc = glGetUniformLocation(postProcess.getProgramId(), "screenTexture");
        glUniform1i(texLoc, 0);

        // Draw the post-processing quad
        glBindVertexArray(postProcess.getQuadVAO());
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Always good practice to set everything back to defaults once configured.
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
    }


    void View::resize(int new_width, int new_height)
    {
        width = new_width;
        height = new_height;

        camera.set_ratio(float(width) / height);

        glViewport(0, 0, width, height);
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
}
