#include "AEGeometry.h"
#include "AEPrimitive.h"
#include "AEDiagnostics.h"

#include <fstream>
#include <iostream>

// Needed for Assimp import objects
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

bool AEGeometry::Import(std::string objFile, AEDrawList& drawList)
{
	DiagTimer perf;
	Assimp::Importer importer;

	// Determin if file exist
	std::ifstream fin(objFile.c_str());
	if (!fin.fail()) {
		fin.close();
	}
	else {
		std::cout << "No import!" << std::endl;
		std::cin.get();
		return NULL;
	}

	// Import file and create pointer to scene
	const aiScene* scene = importer.ReadFile(objFile.c_str(), aiProcessPreset_TargetRealtime_Quality);

	// Check if success with import
	if (!scene)
	{
		std::cout << importer.GetErrorString() << std::endl;
		std::cin.get();
		return NULL;
	}

	unsigned int vertex_count = 0;
	unsigned int indices_count = 0;

	// Construct draw command list
	for (int i(0); i < scene->mNumMeshes; i++)
	{
		unsigned int element_count = scene->mMeshes[i]->mNumFaces * scene->mMeshes[i]->mFaces[0].mNumIndices;

		AEDrawElementsCommand newDraw;
		newDraw.vertexCount = element_count;
		newDraw.instanceCount = 1;
		newDraw.firstIndex = indices_count;
		newDraw.baseVertex = vertex_count;
		newDraw.baseInstance = i;

		drawList.CommandList.push_back(newDraw);
		drawList.IndexList.push_back(i);
		drawList.MatrixList.push_back(glm::scale(ModelMatrix, glm::vec3(0.01f)));

		vertex_count += scene->mMeshes[i]->mNumVertices;
		indices_count += element_count;
	}

	// Allocate memory for array packing
	float* vertex_packed = new float[vertex_count * drawList.vert_data_size];
	float* vertex_packed_start = vertex_packed;
	unsigned int* indices_packed = new unsigned int[indices_count];
	unsigned int* indices_packed_start = indices_packed;

	size_t size_test = sizeof(vertex_packed);

	perf.StartTimer();

	// Pack vertex geometry data
	for (int i(0); i < scene->mNumMeshes; i++)
	{
		for (unsigned int vert_id = 0; vert_id < scene->mMeshes[i]->mNumVertices; vert_id++)
		{
			// Position: vec4
			*vertex_packed = scene->mMeshes[i]->mVertices[vert_id].x;
			vertex_packed++;
			*vertex_packed = scene->mMeshes[i]->mVertices[vert_id].y;
			vertex_packed++;
			*vertex_packed = scene->mMeshes[i]->mVertices[vert_id].z;
			vertex_packed++;
			*vertex_packed = 0.0f;
			vertex_packed++;

			// Vertex Color: vec4
			*vertex_packed = scene->mMeshes[i]->HasVertexColors(0) ? scene->mMeshes[i]->mColors[0]->r : 0.0f;
			vertex_packed++;
			*vertex_packed = scene->mMeshes[i]->HasVertexColors(0) ? scene->mMeshes[i]->mColors[0]->g : 0.0f;
			vertex_packed++;
			*vertex_packed = scene->mMeshes[i]->HasVertexColors(0) ? scene->mMeshes[i]->mColors[0]->b : 0.0f;
			vertex_packed++;
			*vertex_packed = scene->mMeshes[i]->HasVertexColors(0) ? scene->mMeshes[i]->mColors[0]->a : 0.0f;
			vertex_packed++;

			// Vertex Normal: vec4
			*vertex_packed = scene->mMeshes[i]->mNormals[vert_id].x; // (*vert_color).r;
			vertex_packed++;
			*vertex_packed = scene->mMeshes[i]->mNormals[vert_id].y; // (*vert_color).g;
			vertex_packed++;
			*vertex_packed = scene->mMeshes[i]->mNormals[vert_id].z; // (*vert_color).b;
			vertex_packed++;
			*vertex_packed = 0.0f; // (*vert_color).a;
			vertex_packed++;

			// Texture Coordinate: vec2
			*vertex_packed = scene->mMeshes[i]->HasTextureCoords(0) ? scene->mMeshes[i]->mTextureCoords[0]->x : 0.0f;
			vertex_packed++;
			*vertex_packed = scene->mMeshes[i]->HasTextureCoords(0) ? scene->mMeshes[i]->mTextureCoords[0]->y : 0.0f;
			vertex_packed++;
		}

		// Pack indices geometry data
		for (unsigned int face_id = 0; face_id < scene->mMeshes[i]->mNumFaces; face_id++)
		{
			for (unsigned int indice_id = 0; indice_id < scene->mMeshes[i]->mFaces[face_id].mNumIndices; indice_id++)
			{
				*indices_packed = scene->mMeshes[i]->mFaces[face_id].mIndices[indice_id];
				indices_packed++;
			}
		}
	}

	std::cout << "Time packing: " << perf.GetTimer() << "ns" << std::endl;

	// Back to start position of the packed arrays
	vertex_packed = vertex_packed_start;
	vertex_packed_start = NULL;
	indices_packed = indices_packed_start;
	indices_packed_start = NULL;

	drawList.vertex_count = vertex_count;
	drawList.indices_count = indices_count;

	drawList.vertex_data = new float[vertex_count];
	drawList.indices_data = new unsigned int[indices_count];

	// Append data to global buffer
	drawList.vertex_data = vertex_packed;
	drawList.indices_data = indices_packed;

	//std::memcpy(drawList.vertex_data, vertex_packed, vertex_count);
	//std::memcpy(drawList.indices_data, indices_packed, indices_count);

	return true;
}