// Needed for Assimp import objects
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "AEEngine.h"

void AEEngine::ConstructData(AEScene& scene)
{
	DiagTimer						perf;
	Assimp::Importer				importer;
	std::vector<AEImportDataSlice>	ImportedData;

	unsigned int					base_instance = 0;
	unsigned int					vertex_count = 0;
	unsigned int					indices_count = 0;

	float*							vertex_packed_start = nullptr;
	unsigned int*					indices_packed_start = nullptr;

	perf.StartTimer();

	// Import all assets in que
	for (unsigned int asset_id(0); asset_id < scene.ImportList.size(); asset_id++)
	{
		unsigned int this_vertex_count = 0;
		unsigned int this_indices_count = 0;

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

		AEImportDataSlice import_data;

		// Construct draw command list
		for (unsigned int i(0); i < import->mNumMeshes; i++)
		{
			unsigned int element_count = import->mMeshes[i]->mNumFaces * import->mMeshes[i]->mFaces[0].mNumIndices;

			AEDrawElementsCommand newDraw;
			newDraw.vertexCount = element_count;
			newDraw.instanceCount = 1;
			newDraw.firstIndex = indices_count;
			newDraw.baseVertex = vertex_count;
			newDraw.baseInstance = base_instance;

			DrawList.CommandList.push_back(newDraw);
			DrawList.IndexList.push_back(base_instance);
			DrawList.MatrixList.push_back(glm::scale(glm::mat4(1.0f), glm::vec3(0.01f)));

			this_vertex_count += import->mMeshes[i]->mNumVertices;
			this_indices_count += element_count;

			vertex_count += import->mMeshes[i]->mNumVertices;
			indices_count += element_count;

			base_instance++;
		}

		// Store vertex and indices count
		import_data.vertex_count = this_vertex_count;
		import_data.indices_count = this_indices_count;

		// Allocate memory for array packing
		import_data.vertex_data = new float[this_vertex_count * DrawList.vert_data_size];
		import_data.indices_data = new unsigned int[this_indices_count];

		import_data.vertex_data_start = import_data.vertex_data;
		import_data.indices_data_start = import_data.indices_data;

		// Pack vertex geometry data
		for (unsigned int i(0); i < import->mNumMeshes; i++)
		{
			for (unsigned int vert_id = 0; vert_id < import->mMeshes[i]->mNumVertices; vert_id++)
			{
				// Position: vec4
				*import_data.vertex_data = import->mMeshes[i]->mVertices[vert_id].x;
				import_data.vertex_data++;
				*import_data.vertex_data = import->mMeshes[i]->mVertices[vert_id].y;
				import_data.vertex_data++;
				*import_data.vertex_data = import->mMeshes[i]->mVertices[vert_id].z;
				import_data.vertex_data++;
				*import_data.vertex_data = 0.0f;
				import_data.vertex_data++;

				// Vertex Color: vec4
				*import_data.vertex_data = import->mMeshes[i]->HasVertexColors(0) ? import->mMeshes[i]->mColors[0]->r : 0.0f;
				import_data.vertex_data++;
				*import_data.vertex_data = import->mMeshes[i]->HasVertexColors(0) ? import->mMeshes[i]->mColors[0]->g : 0.0f;
				import_data.vertex_data++;
				*import_data.vertex_data = import->mMeshes[i]->HasVertexColors(0) ? import->mMeshes[i]->mColors[0]->b : 0.0f;
				import_data.vertex_data++;
				*import_data.vertex_data = import->mMeshes[i]->HasVertexColors(0) ? import->mMeshes[i]->mColors[0]->a : 0.0f;
				import_data.vertex_data++;

				// Vertex Normal: vec4
				*import_data.vertex_data = import->mMeshes[i]->mNormals[vert_id].x; // (*vert_color).r;
				import_data.vertex_data++;
				*import_data.vertex_data = import->mMeshes[i]->mNormals[vert_id].y; // (*vert_color).g;
				import_data.vertex_data++;
				*import_data.vertex_data = import->mMeshes[i]->mNormals[vert_id].z; // (*vert_color).b;
				import_data.vertex_data++;
				*import_data.vertex_data = 0.0f; // (*vert_color).a;
				import_data.vertex_data++;

				// Texture Coordinate: vec2
				*import_data.vertex_data = import->mMeshes[i]->HasTextureCoords(0) ? import->mMeshes[i]->mTextureCoords[0]->x : 0.0f;
				import_data.vertex_data++;
				*import_data.vertex_data = import->mMeshes[i]->HasTextureCoords(0) ? import->mMeshes[i]->mTextureCoords[0]->y : 0.0f;
				import_data.vertex_data++;
			}

			// Pack indices geometry data
			for (unsigned int face_id = 0; face_id < import->mMeshes[i]->mNumFaces; face_id++)
			{
				for (unsigned int indice_id = 0; indice_id < import->mMeshes[i]->mFaces[face_id].mNumIndices; indice_id++)
				{
					*import_data.indices_data = import->mMeshes[i]->mFaces[face_id].mIndices[indice_id];
					import_data.indices_data++;
				}
			}
		}

		// Back to start position of the packed arrays
		import_data.vertex_data = import_data.vertex_data_start;
		import_data.indices_data = import_data.indices_data_start;

		// Store imported slice
		ImportedData.push_back(import_data);
	}

	DrawList.vertex_count = vertex_count;
	DrawList.indices_count = indices_count;

	DrawList.vertex_data = new float[vertex_count * DrawList.vert_data_size];
	DrawList.indices_data = new unsigned int[indices_count];
	vertex_packed_start = DrawList.vertex_data;
	indices_packed_start = DrawList.indices_data;

	// Pack all data
	for (unsigned int id(0); id < ImportedData.size(); id++)
	{
		unsigned int vertex_data_size = ImportedData[id].vertex_count * DrawList.vert_data_size;
		for (unsigned int v_id(0); v_id < vertex_data_size; v_id++)
		{
			*DrawList.vertex_data = ImportedData[id].vertex_data[v_id];
			DrawList.vertex_data++;
		}
	
		for (unsigned int i_id(0); i_id < ImportedData[id].indices_count; i_id++)
		{
			*DrawList.indices_data = ImportedData[id].indices_data[i_id];
			DrawList.indices_data++;
		}
	}

	DrawList.vertex_data = vertex_packed_start;
	DrawList.indices_data = indices_packed_start;
	vertex_packed_start = NULL;
	indices_packed_start = NULL;

	std::cout << "Time packing: " << perf.GetTimer() << "ns" << std::endl;
}

void AEEngine::CompileVAO()
{
	// Generate geometry buffers
	glCreateVertexArrays(1, &VAO_Static);

	glCreateBuffers(1, &VBO_Static);
	glNamedBufferStorage(VBO_Static, DrawList.vertex_count * DrawList.stride_size, 0, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	vertexArrayPtr = glMapNamedBufferRange(VBO_Static, 0, DrawList.vertex_count * DrawList.stride_size, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

	glCreateBuffers(1, &IBO_Static);
	glNamedBufferStorage(IBO_Static, DrawList.indices_count * sizeof(unsigned int), 0, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	indicesArrayPtr = glMapNamedBufferRange(IBO_Static, 0, DrawList.indices_count * sizeof(unsigned int), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

	glCreateBuffers(1, &DIBO_Static);
	glNamedBufferStorage(DIBO_Static, DrawList.IndexList.size() * sizeof(unsigned int), 0, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	drawIndexesPtr = glMapNamedBufferRange(DIBO_Static, 0, DrawList.IndexList.size() * sizeof(unsigned int), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

	glVertexArrayVertexBuffer(VAO_Static, 0, VBO_Static, 0, 14 * sizeof(float));
	glVertexArrayVertexBuffer(VAO_Static, 1, DIBO_Static, 0, sizeof(unsigned int));
	glVertexArrayElementBuffer(VAO_Static, IBO_Static);

	glEnableVertexArrayAttrib(VAO_Static, VAO_POSITION_LOCATION);
	glEnableVertexArrayAttrib(VAO_Static, VAO_COLOR_LOCATION);
	glEnableVertexArrayAttrib(VAO_Static, VAO_NORMAL_LOCATION);
	glEnableVertexArrayAttrib(VAO_Static, VAO_TEXTURECOORD_LOCATION);
	glEnableVertexArrayAttrib(VAO_Static, VAO_DRAWID_LOCATION);

	glVertexArrayAttribFormat(VAO_Static, VAO_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(VAO_Static, VAO_COLOR_LOCATION, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float));
	glVertexArrayAttribFormat(VAO_Static, VAO_NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float));
	glVertexArrayAttribFormat(VAO_Static, VAO_TEXTURECOORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float));
	glVertexArrayAttribIFormat(VAO_Static, VAO_DRAWID_LOCATION, 1, GL_UNSIGNED_INT, 0);

	glVertexArrayAttribBinding(VAO_Static, VAO_POSITION_LOCATION, 0);
	glVertexArrayAttribBinding(VAO_Static, VAO_COLOR_LOCATION, 0);
	glVertexArrayAttribBinding(VAO_Static, VAO_NORMAL_LOCATION, 0);
	glVertexArrayAttribBinding(VAO_Static, VAO_TEXTURECOORD_LOCATION, 0);
	glVertexArrayAttribBinding(VAO_Static, VAO_DRAWID_LOCATION, 1);

	glVertexArrayBindingDivisor(VAO_Static, 1, 1);

	// Additional buffers
	glGenBuffers(1, &DCBO_Static);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, DCBO_Static);
	glBufferStorage(GL_DRAW_INDIRECT_BUFFER, DrawList.CommandList.size() * sizeof(AEDrawElementsCommand), 0, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	drawCommandPtr = glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, 0, DrawList.CommandList.size() * sizeof(AEDrawElementsCommand), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

	glGenBuffers(1, &ModelMatrixSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ModelMatrixSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, DrawList.MatrixList.size() * sizeof(glm::mat4), DrawList.MatrixList.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ModelMatrixSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	std::memcpy(vertexArrayPtr, DrawList.vertex_data, DrawList.vertex_count * DrawList.stride_size);
	std::memcpy(indicesArrayPtr, DrawList.indices_data, DrawList.indices_count * sizeof(unsigned int));
	std::memcpy(drawIndexesPtr, DrawList.IndexList.data(), DrawList.IndexList.size() * sizeof(unsigned int));
	std::memcpy(drawCommandPtr, DrawList.CommandList.data(), DrawList.CommandList.size() * sizeof(AEDrawElementsCommand));

	glObjectLabel(GL_BUFFER, VAO_Static, -1, "Vertex Array Buffer");
	glObjectLabel(GL_BUFFER, VBO_Static, -1, "Vertex Buffer");
	glObjectLabel(GL_BUFFER, IBO_Static, -1, "Indices Buffer");
	glObjectLabel(GL_BUFFER, DIBO_Static, -1, "Indirect Draw Buffer");
	glObjectLabel(GL_BUFFER, DCBO_Static, -1, "Draw Command Buffer");
	glObjectLabel(GL_BUFFER, ModelMatrixSSBO, -1, "Model Matrix SSBO");
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