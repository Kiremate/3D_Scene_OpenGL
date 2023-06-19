// Mesh.cpp
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
Mesh::Mesh(const std::string& filePath) {
	glGenBuffers(VBO_COUNT, vbo_ids);
	glGenVertexArrays(1, &vao_id);
	load_mesh(filePath);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &vao_id);
	glDeleteBuffers(VBO_COUNT, vbo_ids);
}

void Mesh::render() const {
	glBindVertexArray(vao_id);
	glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}
void Mesh::render(GLuint textureId) const {
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBindVertexArray(vao_id);
	glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);  // Unbind the texture
}
void Mesh::load_mesh(const std::string& mesh_file_path) {
	Assimp::Importer importer;

	auto scene = importer.ReadFile
	(
		mesh_file_path,
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
	);

	if (scene && scene->mNumMeshes > 0)
	{
		auto mesh = scene->mMeshes[0];

		size_t number_of_vertices = mesh->mNumVertices;

		glBindVertexArray(vao_id);

		static_assert(sizeof(aiVector3D) == sizeof(fvec3), "aiVector3D should composed of three floats");

		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
		glBufferData(GL_ARRAY_BUFFER, number_of_vertices * sizeof(aiVector3D), mesh->mVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		if (mesh->HasTextureCoords(0)) { // If the mesh has texture coordinates
			std::vector<vec2> textures(mesh->mNumVertices);
			for (size_t i = 0; i < mesh->mNumVertices; ++i) {
				textures[i] = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			}

			glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[TEXTURE_VBO]);
			glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(vec2), textures.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(2); // This assumes that attribute 0 is for vertex positions and attribute 1 is for colors.
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else { // If the mesh doesn't have texture coordinates, assign random colors to vertices
			std::vector< vec3 > vertex_colors(number_of_vertices);
			for (auto& color : vertex_colors)
			{
				color = random_color();
			}

			glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
			glBufferData(GL_ARRAY_BUFFER, vertex_colors.size() * sizeof(vec3), vertex_colors.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		number_of_indices = mesh->mNumFaces * 3;

		std::vector< GLshort > indices(number_of_indices);

		auto vertex_index = indices.begin();

		for (unsigned i = 0; i < mesh->mNumFaces; ++i)
		{
			auto& face = mesh->mFaces[i];

			assert(face.mNumIndices == 3);

			*vertex_index++ = face.mIndices[0];
			*vertex_index++ = face.mIndices[1];
			*vertex_index++ = face.mIndices[2];
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLshort), indices.data(), GL_STATIC_DRAW);
	}
}


vec3 Mesh::random_color() {
	return vec3
	(
		float(rand()) / float(RAND_MAX),
		float(rand()) / float(RAND_MAX),
		float(rand()) / float(RAND_MAX)
	);
}
