#include "PostProcess.h"
#include <cassert>
#include <iostream>
#include "ShaderUtility.h"

PostProcess::PostProcess(FrameBuffer& fb) : frameBuffer(fb) {
	// Initialization of quadVertices
	float vertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	memcpy(quadVertices, vertices, sizeof(vertices));

	// Initialization of postprocess_vertex_shader_code
	postprocess_vertex_shader_code =
		"#version 330\n"
		"in vec2 position;"
		"in vec2 TexCoords;"
		"out vec2 TexCoords0;"
		"void main()"
		"{"
		"   gl_Position = vec4(position, 0.0f, 1.0f);"
		"   TexCoords0 = TexCoords;"
		"}";

	// Initialization of postprocess_fragment_shader_code
	postprocess_fragment_shader_code =
		"#version 330\n"
		"in vec2 TexCoords0;"
		"out vec4 color;"
		"uniform sampler2D screenTexture;"
		"void main()"
		"{"
		"   vec3 texColor = texture(screenTexture, TexCoords0).rgb;"
		"   float gray = dot(texColor, vec3(0.3, 0.59, 0.11));"
		"   color = vec4(vec3(gray), 1.0f);"
		"}";


	// Compilation of shaders
	postprocess_program_id = ShaderUtility::CompileShaders(postprocess_vertex_shader_code, postprocess_fragment_shader_code);
}

void PostProcess::Bind() {
	frameBuffer.Bind();
}

void PostProcess::Unbind() {
	frameBuffer.Unbind();
}

void PostProcess::Init() {
	// Setup of quad VAO and VBO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);  
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0); 
}


void PostProcess::Process() {
	// Apply postprocessing:
	glUseProgram(postprocess_program_id);
	glBindTexture(GL_TEXTURE_2D, GetTextureBuffer());
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

GLuint PostProcess::getProgramId() const
{
	return postprocess_program_id;
}

GLuint PostProcess::GetTextureBuffer() {
	return frameBuffer.GetTextureBuffer();
}

GLuint PostProcess::getQuadVAO() const
{
	return quadVAO;
}
