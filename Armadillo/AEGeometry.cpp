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

bool AEGeometry::Init()
{
	DiagTimer perf;
	AEPrimitive plane(eAE_PrimitiveType_Plane);

	Assimp::Importer importer;
	const std::string objFile = "./resources/meshes/Sponza/Sponza.gltf";
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

	unsigned int vert_data_size = 14;
	unsigned int stride_size = vert_data_size * sizeof(float);

	unsigned int vertex_count = 0;
	unsigned int indices_count = 0;
	mesh_count = scene->mNumMeshes;

	std::vector<AEDrawElementsCommand> vDrawCommand;
	std::vector<unsigned int> vMatrixId;

	for (int i(0); i < mesh_count; i++)
	{
		unsigned int element_count = scene->mMeshes[i]->mNumFaces * scene->mMeshes[i]->mFaces[0].mNumIndices;

		AEDrawElementsCommand newDraw;
		newDraw.vertexCount = element_count;
		newDraw.instanceCount = 1;
		newDraw.firstIndex = indices_count;
		newDraw.baseVertex = vertex_count;
		newDraw.baseInstance = i;
		vDrawCommand.push_back(newDraw);
		vMatrixId.push_back(0);

		vertex_count += scene->mMeshes[i]->mNumVertices;
		indices_count += element_count;
	}

	// Allocate memory for array packing
	float* vertex_packed = new float[vertex_count * vert_data_size];
	float* vertex_packed_start = vertex_packed;
	unsigned int* indices_packed = new unsigned int[indices_count];
	unsigned int* indices_packed_start = indices_packed;

	perf.StartTimer();

	// Pack vertex geometry data
	for (int i(0); i < mesh_count; i++)
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

	// Generate geometry buffers
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	glGenBuffers(1, &indirectDrawBuffer);
	glGenBuffers(1, &iid);

	glBindVertexArray(vao);
	glObjectLabel(GL_BUFFER, vao, -1, "Vertex Array Buffer");
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(VAO_POSITION_LOCATION);
	glVertexAttribPointer(VAO_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, stride_size, 0);

	glEnableVertexAttribArray(VAO_COLOR_LOCATION);
	glVertexAttribPointer(VAO_COLOR_LOCATION, 3, GL_FLOAT, GL_FALSE, stride_size, (void*)(4 * sizeof(float)));

	glEnableVertexAttribArray(VAO_NORMAL_LOCATION);
	glVertexAttribPointer(VAO_NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, stride_size, (void*)(8 * sizeof(float)));

	glEnableVertexAttribArray(VAO_TEXTURECOORD_LOCATION);
	glVertexAttribPointer(VAO_TEXTURECOORD_LOCATION, 2, GL_FLOAT, GL_FALSE, stride_size, (void*)(12 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex_count * stride_size, vertex_packed, GL_STATIC_DRAW);
	glObjectLabel(GL_BUFFER, vbo, -1, "Vertex Buffer");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_count * sizeof(unsigned int), indices_packed, GL_STATIC_DRAW);
	glObjectLabel(GL_BUFFER, ibo, -1, "Indices Buffer");
	
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectDrawBuffer);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, mesh_count * sizeof(AEDrawElementsCommand), &vDrawCommand[0], GL_STATIC_DRAW);
	glObjectLabel(GL_BUFFER, indirectDrawBuffer, -1, "Indirect Draw Buffer");
	
	glBindBuffer(GL_ARRAY_BUFFER, iid);
	glBufferData(GL_ARRAY_BUFFER, mesh_count * sizeof(unsigned int), &vMatrixId[0], GL_STATIC_DRAW);
	glObjectLabel(GL_BUFFER, iid, -1, "Matrix ID Buffer");

	glEnableVertexAttribArray(VAO_DRAWID_LOCATION);
	glVertexAttribIPointer(VAO_DRAWID_LOCATION, 1, GL_UNSIGNED_INT, 0, (void*)0);
	glVertexAttribDivisor(VAO_DRAWID_LOCATION, 1);

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