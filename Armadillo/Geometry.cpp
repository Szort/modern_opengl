#include "Geometry.h"
#include "AEPrimitive.h"
#include <fstream>
#include <iostream>

// Needed for Assimp import objects
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

bool Geometry::Init()
{
	AEPrimitive plane(eAE_PrimitiveType_Plane);

	Assimp::Importer importer;
	const std::string objFile = "./Objects/TriangleWithoutIndices.gltf";
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
	aiVector3D* vertices = triangle->mVertices;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, triangle->mNumVertices * sizeof(aiVector3D), &vertices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	if (vbo != 0 && vao != 0)
		return true;
	else
		return false;
}

void Geometry::BindGeometry()
{
	glBindVertexArray(vao);
}

void Geometry::UnbindGeometry()
{
	glBindVertexArray(0);
}