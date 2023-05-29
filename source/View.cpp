
// Este código es de dominio público.
// angel.rodriguez@esne.edu
// 2013.12 - 2021.04

#include <cassert>
#include <cmath>
#include <glad/glad.h>
#include "math.hpp"
#include "View.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef DEBUG
#define CHECK_GL_ERROR(op) checkGlError(op)
#else
#define CHECK_GL_ERROR(op)
#endif


View::View(float width, float height)
	: width(width), height(height)
{
	// Generate and bind Vertex Array Object
	glDisable(GL_CULL_FACE);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Simple triangle data
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	// Generate and bind Vertex Buffer Object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Setup the vertex attributes pointers.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Compile and use shaders
	shaderProgram = compile_shaders();
	glUseProgram(shaderProgram);

	// Initialize Camera
	// Adjust the values here to fit your specific needs
	float fov = 45.0f;
	float near_plane = 0.1f;
	float far_plane = 100.0f;
	example::Vector3f position(0.0f, 0.0f, 0.0f);
	example::Vector3f target(0.0f, 0.0f, -1.0f);
	example::Vector3f up(0.0f, 1.0f, 0.0f);
	camera = Camera(fov, near_plane, far_plane, position, target, up);
	// Initialize Light
	Color lightColor = { 1.0f, 1.0f, 1.0f }; // white light
	light.position = camera.transform.position;
	light.view = camera.view_direction;
	light.color = lightColor;
}
View::~View()
{
	// Delete the VAO, VBO, and IBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);

	// Delete the shader program
	glDeleteProgram(shaderProgram);

	// Delete textures
	// You should probably store all your texture ids in a std::vector or similar container
	// to be able to delete them all here.
	// In this case, I'll just delete the texture named "texture"
	GLuint texture = textureManager.getTexture("texture");
	glDeleteTextures(1, &texture);
}

GLuint View::compile_shaders()
{
	GLint succeeded = GL_FALSE;

	// Create shader objects:
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	// Load shader source code from files:


	const std::string vertex_shader_code =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aNormal;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"out vec3 FragPos;\n"
		"out vec3 Normal;\n"
		"void main()\n"
		"{\n"
		"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
		"   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
		"   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
		"}";

	const std::string fragment_shader_code =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec3 lightPos;\n"
		"uniform vec3 lightColor;\n"
		"uniform vec3 viewPos;\n"
		"uniform vec3 ambientColor;\n"
		"in vec3 FragPos;\n"
		"in vec3 Normal;\n"
		"void main()\n"
		"{\n"
		"   // Ambient lighting\n"
		"   float ambientStrength = 0.1f;\n"
		"   vec3 ambient = ambientStrength * ambientColor;\n"

		"   // Diffuse lighting\n"
		"   vec3 norm = normalize(Normal);\n"
		"   vec3 lightDir = normalize(lightPos - FragPos);\n"
		"   float diff = max(dot(norm, lightDir), 0.0);\n"
		"   vec3 diffuse = diff * lightColor;\n"

		"   // Resulting color\n"
		"   vec3 result = (ambient + diffuse);\n"
		"   FragColor = vec4(result, 1.0f);\n"
		"}";



	//std::string vertex_shader_code = loadShaderSource("../../shared/assets/shaders/vertex_shader.glsl");
	//std::string fragment_shader_code = loadShaderSource("../../shared/assets/shaders/fragment_shader.glsl");
	// Safe check
	if (vertex_shader_code.empty() || fragment_shader_code.empty()) {
		std::cerr << "Failed to load shader source code from file." << std::endl;
		exit(EXIT_FAILURE);
	}
	const GLchar* vertex_shaders_code_ptr = vertex_shader_code.c_str();
	const GLchar* fragment_shaders_code_ptr = fragment_shader_code.c_str();

	glShaderSource(vertex_shader_id, 1, &vertex_shaders_code_ptr, NULL);
	glShaderSource(fragment_shader_id, 1, &fragment_shaders_code_ptr, NULL);

	// Compile shaders:
	glCompileShader(vertex_shader_id);
	glCompileShader(fragment_shader_id);

	// Check if compilation was successful:
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
	if (!succeeded) show_compilation_error(vertex_shader_id);

	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
	if (!succeeded) show_compilation_error(fragment_shader_id);

	// Create a program object:
	GLuint program_id = glCreateProgram();

	// Load compiled shaders into the program:
	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);

	// Link shaders:
	glLinkProgram(program_id);

	// Check if linkage was successful:
	glGetProgramiv(program_id, GL_LINK_STATUS, &succeeded);
	if (!succeeded) show_linkage_error(program_id);

	// Delete compiled shaders once they have been linked:
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	return program_id;
}

void View::show_linkage_error(GLuint program_id)
{
	std::string info_log;
	GLint  info_log_length;

	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);

	info_log.resize(info_log_length);

	glGetProgramInfoLog(program_id, info_log_length, NULL, &info_log.front());

	std::cerr << info_log.c_str() << std::endl;

#ifdef _MSC_VER
	//OutputDebugStringA (info_log.c_str ());
#endif

	assert(false);
}

void View::show_compilation_error(GLuint shader_id)
{
	std::string info_log;
	GLint  info_log_length;

	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

	info_log.resize(info_log_length);

	glGetShaderInfoLog(shader_id, info_log_length, NULL, &info_log.front());

	std::cerr << info_log.c_str() << std::endl;

#ifdef _MSC_VER
	//OutputDebugStringA (info_log.c_str ());
#endif

	assert(false);
}

void View::update()
{
	for (auto& mesh : meshes)
		mesh.transform_vertices(camera, width, height);
}

Camera& View::get_camera()
{
	return camera;
}

void View::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);

	// get uniform locations
	GLint lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");
	GLint viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos");

	// set uniform values
	glUniform3f(lightPosLocation, light.position.x, light.position.y, light.position.z);
	glUniform3f(viewPosLocation, camera.transform.position.x, camera.transform.position.y, camera.transform.position.z);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



void checkGlError(const char* op) {
	for (GLint error = glGetError(); error; error = glGetError()) {
		std::cerr << "After " << op << " glError (0x" << std::hex << error << ")\n";
	}
}

std::string View::loadShaderSource(const std::string& shaderFilePath)
{
	std::ifstream shaderFile(shaderFilePath);
	if (!shaderFile.good())
	{
		std::cerr << "Failed to open shader file: " << shaderFilePath << std::endl;
		return "";
	}

	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	return shaderStream.str();
}

