// Needed for Assimp import objects
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "AEEngine.h"

void AEEngine::ConstructData(AEScene& scene)
{
	DiagTimer perf;
	Assimp::Importer importer;

	unsigned int vertex_count = 0;
	unsigned int indices_count = 0;

	for (unsigned int asset_id(0); asset_id < scene.ImportList.size(); asset_id++)
	{
		// Import file and create pointer to scene
		const aiScene* import = importer.ReadFile(scene.ImportList[asset_id].c_str(), aiProcessPreset_TargetRealtime_Quality);
		
		// Check if success with import
		if (!import)
		{
			std::cout << importer.GetErrorString() << std::endl;
			std::cin.get();
			continue;
		}

		//Check if asset have any meshes
		if (!import->HasMeshes())
		{
			std::cout << "No meshes imported from file: " << scene.ImportList[asset_id] << std::endl;
			continue;
		}

		perf.StartTimer();

		// Construct draw command list
		for (unsigned int i(0); i < import->mNumMeshes; i++)
		{
			unsigned int element_count = import->mMeshes[i]->mNumFaces * import->mMeshes[i]->mFaces[0].mNumIndices;

			AEDrawElementsCommand newDraw;
			newDraw.vertexCount = element_count;
			newDraw.instanceCount = 1;
			newDraw.firstIndex = indices_count;
			newDraw.baseVertex = vertex_count;
			newDraw.baseInstance = i;

			DrawList.CommandList.push_back(newDraw);
			DrawList.IndexList.push_back(i);
			DrawList.MatrixList.push_back(glm::scale(glm::mat4(1.0f), glm::vec3(0.01f)));

			vertex_count += import->mMeshes[i]->mNumVertices;
			indices_count += element_count;
		}

		DrawList.vertex_count = vertex_count;
		DrawList.indices_count = indices_count;

		// Allocate memory for array packing
		DrawList.vertex_data = new float[vertex_count * DrawList.vert_data_size];
		float* vertex_packed_start = DrawList.vertex_data;
		DrawList.indices_data = new unsigned int[indices_count];
		unsigned int* indices_packed_start = DrawList.indices_data;

		// Pack vertex geometry data
		for (unsigned int i(0); i < import->mNumMeshes; i++)
		{
			for (unsigned int vert_id = 0; vert_id < import->mMeshes[i]->mNumVertices; vert_id++)
			{
				// Position: vec4
				*DrawList.vertex_data = import->mMeshes[i]->mVertices[vert_id].x;
				DrawList.vertex_data++;
				*DrawList.vertex_data = import->mMeshes[i]->mVertices[vert_id].y;
				DrawList.vertex_data++;
				*DrawList.vertex_data = import->mMeshes[i]->mVertices[vert_id].z;
				DrawList.vertex_data++;
				*DrawList.vertex_data = 0.0f;
				DrawList.vertex_data++;

				// Vertex Color: vec4
				*DrawList.vertex_data = import->mMeshes[i]->HasVertexColors(0) ? import->mMeshes[i]->mColors[0]->r : 0.0f;
				DrawList.vertex_data++;
				*DrawList.vertex_data = import->mMeshes[i]->HasVertexColors(0) ? import->mMeshes[i]->mColors[0]->g : 0.0f;
				DrawList.vertex_data++;
				*DrawList.vertex_data = import->mMeshes[i]->HasVertexColors(0) ? import->mMeshes[i]->mColors[0]->b : 0.0f;
				DrawList.vertex_data++;
				*DrawList.vertex_data = import->mMeshes[i]->HasVertexColors(0) ? import->mMeshes[i]->mColors[0]->a : 0.0f;
				DrawList.vertex_data++;

				// Vertex Normal: vec4
				*DrawList.vertex_data = import->mMeshes[i]->mNormals[vert_id].x; // (*vert_color).r;
				DrawList.vertex_data++;
				*DrawList.vertex_data = import->mMeshes[i]->mNormals[vert_id].y; // (*vert_color).g;
				DrawList.vertex_data++;
				*DrawList.vertex_data = import->mMeshes[i]->mNormals[vert_id].z; // (*vert_color).b;
				DrawList.vertex_data++;
				*DrawList.vertex_data = 0.0f; // (*vert_color).a;
				DrawList.vertex_data++;

				// Texture Coordinate: vec2
				*DrawList.vertex_data = import->mMeshes[i]->HasTextureCoords(0) ? import->mMeshes[i]->mTextureCoords[0]->x : 0.0f;
				DrawList.vertex_data++;
				*DrawList.vertex_data = import->mMeshes[i]->HasTextureCoords(0) ? import->mMeshes[i]->mTextureCoords[0]->y : 0.0f;
				DrawList.vertex_data++;
			}

			// Pack indices geometry data
			for (unsigned int face_id = 0; face_id < import->mMeshes[i]->mNumFaces; face_id++)
			{
				for (unsigned int indice_id = 0; indice_id < import->mMeshes[i]->mFaces[face_id].mNumIndices; indice_id++)
				{
					*DrawList.indices_data = import->mMeshes[i]->mFaces[face_id].mIndices[indice_id];
					DrawList.indices_data++;
				}
			}
		}

		// Back to start position of the packed arrays
		DrawList.vertex_data = vertex_packed_start;
		vertex_packed_start = NULL;
		DrawList.indices_data = indices_packed_start;
		indices_packed_start = NULL;

		std::cout << "Time packing: " << perf.GetTimer() << "ns" << std::endl;
	}
}

void AEEngine::CompileVAO()
{
	// Generate geometry buffers
	glGenVertexArrays(1, &VAO_Static);
	glGenBuffers(1, &VBO_Static);
	glGenBuffers(1, &IBO_Static);
	glGenBuffers(1, &DCBO_Static);
	glGenBuffers(1, &DIBO_Static);

	glBindVertexArray(VAO_Static);
	glObjectLabel(GL_BUFFER, VAO_Static, -1, "Vertex Array Buffer");

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Static);

	glEnableVertexAttribArray(VAO_POSITION_LOCATION);
	glVertexAttribPointer(VAO_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, DrawList.stride_size, 0);

	glEnableVertexAttribArray(VAO_COLOR_LOCATION);
	glVertexAttribPointer(VAO_COLOR_LOCATION, 3, GL_FLOAT, GL_FALSE, DrawList.stride_size, (void*)(4 * sizeof(float)));

	glEnableVertexAttribArray(VAO_NORMAL_LOCATION);
	glVertexAttribPointer(VAO_NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, DrawList.stride_size, (void*)(8 * sizeof(float)));

	glEnableVertexAttribArray(VAO_TEXTURECOORD_LOCATION);
	glVertexAttribPointer(VAO_TEXTURECOORD_LOCATION, 2, GL_FLOAT, GL_FALSE, DrawList.stride_size, (void*)(12 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Static);
	glBufferData(GL_ARRAY_BUFFER, DrawList.vertex_count * DrawList.stride_size, DrawList.vertex_data, GL_STATIC_DRAW);
	glObjectLabel(GL_BUFFER, VBO_Static, -1, "Vertex Buffer");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_Static);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, DrawList.indices_count * sizeof(unsigned int), DrawList.indices_data, GL_STATIC_DRAW);
	glObjectLabel(GL_BUFFER, IBO_Static, -1, "Indices Buffer");

	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, DCBO_Static);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, DrawList.CommandList.size() * sizeof(AEDrawElementsCommand), &DrawList.CommandList[0], GL_STATIC_DRAW);
	glObjectLabel(GL_BUFFER, DCBO_Static, -1, "Indirect Draw Buffer");

	glBindBuffer(GL_ARRAY_BUFFER, DIBO_Static);
	glBufferData(GL_ARRAY_BUFFER, DrawList.CommandList.size() * sizeof(unsigned int), &DrawList.IndexList[0], GL_STATIC_DRAW);
	glObjectLabel(GL_BUFFER, DIBO_Static, -1, "Matrix ID Buffer");

	glEnableVertexAttribArray(VAO_DRAWID_LOCATION);
	glVertexAttribIPointer(VAO_DRAWID_LOCATION, 1, GL_UNSIGNED_INT, 0, (void*)0);
	glVertexAttribDivisor(VAO_DRAWID_LOCATION, 1);
}

void AEEngine::Idle()
{
	// Sleeping time
	SleepTime = (1000 / FpsCap) - RenderTime;
	SleepTime = SleepTime < 0 ? 0 : SleepTime;
	Sleep(SleepTime);
}

void AEEngine::BindVAO()
{
	glBindVertexArray(VAO_Static);
}

void AEEngine::UnbindVAO()
{
	glBindVertexArray(0);
}