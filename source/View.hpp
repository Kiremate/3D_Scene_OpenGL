#pragma once

#include <Color_Buffer.hpp>
#include <cstdlib>
#include "math.hpp"
#include <vector>
#include "Camera.h"
#include "Mesh.h"
#include "TextureManager.h"

class View
{
private:
	typedef Rgb888                Color;
	typedef Color_Buffer< Color > Color_Buffer;
	typedef Point4f               Vertex;
	typedef std::vector< Vertex >      Vertex_Buffer;
	typedef std::vector< int    >      Index_Buffer;
	typedef std::vector< Color  >      Vertex_Colors;

	// OpenGL resources
	GLuint VAO, VBO, IBO;
	GLuint shaderProgram;
	std::vector<Mesh> meshes;
	Camera camera;
	unsigned width;
	unsigned height;

public:
	struct Light
	{
		example::Vector3f position;
		Color color;
		Light(const example::Vector3f& position_, const Color& color_)
			: position(position_), color(color_)
		{}
	};
	View(float width, float height);
	~View();
	TextureManager textureManager;

	void update();
	void render();
	Camera& get_camera();
private:
	Light light;
	Color ambient_color;
	bool  is_frontface(const Vertex* const projected_vertices, const int* const indices);
	float rand_clamp() { return float(rand() & 0xff) * 0.0039215f; }
	GLuint compile_shaders();
	void show_compilation_error(GLuint  shader_id);
	void show_linkage_error(GLuint program_id);
	std::string loadShaderSource(const std::string& shaderFilePath);
};

