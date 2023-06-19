
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
#include "TextureManager.h"
#include "Node.h"
#include "FrameBuffer.h"
#include "PostProcess.h"
#include "MeshDataTypes.h"

namespace example
{

    using glm::vec3;
    class View
    {
    private:
        Camera camera;
        Skybox skybox;
        FrameBuffer frameBuffer;
        std::shared_ptr<Node> rootNode;
        std::shared_ptr<Node> bunnyNodeTexture;
        GLuint program_id;
        GLuint program_id_texture;
        PostProcess postProcess;
        int    width;
        int    height;
        float  angle_around_x;
        float  angle_around_y;
        float  angle_delta_x;
        float  angle_delta_y;
        bool   pointer_pressed;
        int    last_pointer_x;
        int    last_pointer_y;
        TextureManager textureManager;
        static const std::string   vertex_shader_code;
        static const std::string fragment_shader_code;  
        static const std::string   vertex_shader_code_texture;
        static const std::string fragment_shader_code_texture;
        GLint   model_view_matrix_id;
        GLint   projection_matrix_id;
        bool post_processing_enabled = true;
        float   angle;
        void beginRender();
        void renderSkybox();
        void renderOpaqueObjects();
        void renderTransparentObjects();
        void endRender();
        void handlePostProcessing();
        void resetOpenGLState();

    public:

        View(int width, int height);
        ~View();

        void   update();
        void   render();
        void   resize(int width, int height);
        void on_key(int key_code);
        void on_drag(int pointer_x, int pointer_y);
        void on_click(int pointer_x, int pointer_y, bool down);
    };
}
#endif
