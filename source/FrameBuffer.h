#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

class FrameBuffer
{
public:
    FrameBuffer(int width, int height);
    ~FrameBuffer();

    void Bind();
    void Unbind();
    GLuint GetTextureBuffer();

private:
    GLuint fbo;
    GLuint textureColorbuffer;
    GLuint rbo;
};
