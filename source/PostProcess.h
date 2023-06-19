#pragma once
#include "FrameBuffer.h"

class PostProcess{

public:

    PostProcess(FrameBuffer& frameBuffer);

    void Bind();    // Bind the framebuffer
    void Unbind();  // Unbind the framebuffer

    void Init();    // Setup of quad VAO and VBO
    void Process(); // Apply postprocessing

    GLuint getProgramId() const;
    GLuint getQuadVAO() const;
    GLuint GetTextureBuffer(); // Return texture buffer from framebuffer

private:
    FrameBuffer& frameBuffer;
    GLuint quadVAO;
    GLuint quadVBO;
    GLuint postprocess_program_id;
    std::string postprocess_vertex_shader_code;
    std::string postprocess_fragment_shader_code;
    float quadVertices[24];
};
