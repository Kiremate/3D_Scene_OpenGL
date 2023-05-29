// Mesh.cpp
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh::Mesh(const std::string& file_path)
{
    Assimp::Importer importer;

    auto scene = importer.ReadFile
    (
        file_path.c_str(),
        aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
    );

    if (scene && scene->mNumMeshes > 0)
    {
        auto mesh = scene->mMeshes[0];

        size_t number_of_vertices = mesh->mNumVertices;

        original_vertices.resize(number_of_vertices);
        transformed_vertices.resize(number_of_vertices);

        for (size_t index = 0; index < number_of_vertices; index++)
        {
            auto& vertex = mesh->mVertices[index];

            original_vertices[index] = example::Vertex(vertex.x, -vertex.y, vertex.z, 1.f);
        }

        if (mesh->HasTextureCoords(0)) // assuming the UVs are in channel 0
        {
            original_uvs.resize(number_of_vertices);

            for (size_t index = 0; index < number_of_vertices; index++)
            {
                auto& uv = mesh->mTextureCoords[0][index];
                original_uvs[index] = example::UV(uv.x, uv.y);
            }
        }

        size_t number_of_triangles = mesh->mNumFaces;

        original_indices.resize(number_of_triangles * 3);

        example::Index_Buffer::iterator indices_iterator = original_indices.begin();

        for (size_t index = 0; index < number_of_triangles; index++)
        {
            auto& face = mesh->mFaces[index];

            assert(face.mNumIndices == 3);

            auto indices = face.mIndices;

            *indices_iterator++ = int(indices[0]);
            *indices_iterator++ = int(indices[1]);
            *indices_iterator++ = int(indices[2]);
        }

        original_colors.resize(number_of_vertices);

        for (size_t index = 0; index < number_of_vertices; index++)
        {
            original_colors[index].set(rand() % 256 / 255.f, rand() % 256 / 255.f, rand() % 256 / 255.f);
        }
    }
}

void Mesh::transform_vertices(Camera& camera, float width, float height)
{
    // Se crean las matrices de transformación:
    Matrix44 identity(1);
    Matrix44 scaling = scale(identity, 4.f);

    // Use an identity matrix instead of rotation_y to disable rotation
    //Matrix44 rotation_y = identity;
    Matrix44 rotation_y = rotate_around_y(identity, 0.025f);

    Matrix44 translation = translate(identity, Vector3f{ 0.f, 0.5f, -10.f });
    Matrix44 projection = perspective(20, 1, 15, float(width) / height);

    // Get the camera's view matrix
    Matrix44 view_matrix = camera.get_view_matrix();

    // Creación de la matriz de transformación unificada:
    Matrix44 transformation = projection * view_matrix * translation * rotation_y * scaling;

    // Se transforman todos los vértices usando la matriz de transformación resultante:
    for (size_t index = 0, number_of_vertices = original_vertices.size(); index < number_of_vertices; index++)
    {
        // Se multiplican todos los vértices originales con la matriz de transformación y
        // se guarda el resultado en otro vertex buffer:
        Vertex& vertex = transformed_vertices[index] = transformation * original_vertices[index];

        // La matriz de proyección en perspectiva hace que el último componente del vector
        // transformado no tenga valor 1.0, por lo que hay que normalizarlo dividiendo:
        float divisor = 1.f / vertex.w;

        vertex.x *= divisor;
        vertex.y *= divisor;
        vertex.z *= divisor;
        vertex.w = 1.f;
    }
}
