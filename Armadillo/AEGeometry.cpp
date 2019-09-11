#include "AEGeometry.h"
#include "AEPrimitive.h"
#include <fstream>
#include <iostream>

// Needed for Assimp import objects
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace AE
{
	extern int fpsCup;
}

bool AEGeometry::Init()
{
	AEPrimitive plane(eAE_PrimitiveType_Plane);

	Assimp::Importer importer;
	//const std::string objFile = "./Objects/TriangleWithoutIndices.gltf";
	const std::string objFile = "./Objects/BoxVertexColors.gltf";
	const aiScene* scene = NULL;

	std::ifstream fin(objFile.c_str());
	if (!fin.fail())
	{
		fin.close();
		std::cout << "file exist!" << std::endl;
	}
	else {
		std::cout << "No import!" << std::endl;
		std::cin.get();
		return NULL;
	}

	scene = importer.ReadFile(objFile.c_str(), aiProcessPreset_TargetRealtime_Quality);

	if (!scene)
	{
		std::cout << importer.GetErrorString() << std::endl;
		std::cin.get();
		return NULL;
	}

	std::cout << "Number of imported meshes: " << scene->mNumMeshes << std::endl;

	aiMesh* triangle = scene->mMeshes[0];
	aiVector3D* vert_pos = triangle->mVertices;
	aiColor4D* vert_color = triangle->mColors[0];
	aiFace* faces = triangle->mFaces;
	unsigned int indices_size = triangle->mNumFaces * faces->mNumIndices;

	unsigned int vert_data_size = 8;
	unsigned int stride_size = vert_data_size * sizeof(float);

	// Pack vertices data like position, color, normal, etc.
	float* packed_vertices = new float[triangle->mNumVertices * vert_data_size];
	// Get start address of packed array 
	float* start = packed_vertices;

	unsigned int pos = 0;
	for (unsigned int vert_id = 0; vert_id < triangle->mNumVertices; vert_id++)
	{
		// Position
		*packed_vertices = (*vert_pos).x;
		packed_vertices++;
		*packed_vertices = (*vert_pos).y;
		packed_vertices++;
		*packed_vertices = (*vert_pos).z;
		packed_vertices++;
		*packed_vertices = 0.0f;
		packed_vertices++;
		// Color
		*packed_vertices = (*vert_color).r;
		packed_vertices++;
		*packed_vertices = (*vert_color).g;
		packed_vertices++;
		*packed_vertices = (*vert_color).b;
		packed_vertices++;
		*packed_vertices = (*vert_color).a;
		packed_vertices++;

		// Go to next vertex
		vert_pos++;
		vert_color++;
	}

	// Back to start possition of the array
	packed_vertices = start;
	start = NULL;

	// Get indices data for this mesh
	unsigned int* indices_list = new unsigned int[indices_size];
	for (unsigned int face_id = 0; face_id < triangle->mNumFaces; face_id++)
	{
		for (unsigned int indice_id = 0; indice_id < faces->mNumIndices; indice_id++)
			indices_list[3 * face_id + indice_id] = faces->mIndices[indice_id];

		faces++;
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride_size, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride_size, (void*)(4 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, triangle->mNumVertices * stride_size, packed_vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size * sizeof(unsigned int), indices_list, GL_STATIC_DRAW);

	if (vbo != 0 && vao != 0 && ibo != 0)
		return true;
	else
		return false;
}

void AEGeometry::BindGeometry()
{
	glBindVertexArray(vao);
}

void AEGeometry::UnbindGeometry()
{
	glBindVertexArray(0);
}