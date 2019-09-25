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

	uint32_t					base_instance = 0;
	uint32_t					vertex_count = 0;
	uint32_t					indices_count = 0;

	float*							vertex_packed_start = nullptr;
	uint32_t*					indices_packed_start = nullptr;

	perf.StartTimer();

	// Import all assets in que
	uint32_t this_vertex_count;
	uint32_t this_indices_count;

	for (uint32_t asset_id(0); asset_id < scene.ImportList.size(); asset_id++)
	{
		this_vertex_count = 0;
		this_indices_count = 0;

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
		for (uint32_t i(0); i < import->mNumMeshes; i++)
		{
			uint32_t element_count = import->mMeshes[i]->mNumFaces * import->mMeshes[i]->mFaces[0].mNumIndices;

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
		import_data.indices_data = new uint32_t[this_indices_count];

		import_data.vertex_data_start = import_data.vertex_data;
		import_data.indices_data_start = import_data.indices_data;

		// Pack vertex geometry data
		for (uint32_t i(0); i < import->mNumMeshes; i++)
		{
			for (uint32_t vert_id = 0; vert_id < import->mMeshes[i]->mNumVertices; vert_id++)
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
			for (uint32_t face_id = 0; face_id < import->mMeshes[i]->mNumFaces; face_id++)
			{
				for (uint32_t indice_id = 0; indice_id < import->mMeshes[i]->mFaces[face_id].mNumIndices; indice_id++)
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
	DrawList.indices_data = new uint32_t[indices_count];
	vertex_packed_start = DrawList.vertex_data;
	indices_packed_start = DrawList.indices_data;

	// Pack all data
	for (uint32_t id(0); id < ImportedData.size(); id++)
	{
		uint32_t vertex_data_size = ImportedData[id].vertex_count * DrawList.vert_data_size;
		for (uint32_t v_id(0); v_id < vertex_data_size; v_id++)
		{
			*DrawList.vertex_data = ImportedData[id].vertex_data[v_id];
			DrawList.vertex_data++;
		}
	
		for (uint32_t i_id(0); i_id < ImportedData[id].indices_count; i_id++)
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
	uint32_t flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	// Generate geometry buffers
	glCreateVertexArrays(1, &VertexArrayObject);

	glCreateBuffers(1, &VertexBufferObject);
	glNamedBufferStorage(VertexBufferObject, DrawList.vertex_count * DrawList.stride_size, 0, flags);
	vertexArrayPtr = glMapNamedBufferRange(VertexBufferObject, 0, DrawList.vertex_count * DrawList.stride_size, flags);

	glCreateBuffers(1, &IndicesBufferObject);
	glNamedBufferStorage(IndicesBufferObject, DrawList.indices_count * sizeof(uint32_t), 0, flags);
	indicesArrayPtr = glMapNamedBufferRange(IndicesBufferObject, 0, DrawList.indices_count * sizeof(uint32_t), flags);

	glCreateBuffers(1, &DrawIndexObject);
	glNamedBufferStorage(DrawIndexObject, DrawList.IndexList.size() * sizeof(uint32_t), 0, flags);
	drawIndexesPtr = glMapNamedBufferRange(DrawIndexObject, 0, DrawList.IndexList.size() * sizeof(uint32_t), flags);

	glVertexArrayVertexBuffer(VertexArrayObject, 0, VertexBufferObject, 0, 14 * sizeof(float));
	glVertexArrayVertexBuffer(VertexArrayObject, 1, DrawIndexObject, 0, sizeof(uint32_t));
	glVertexArrayElementBuffer(VertexArrayObject, IndicesBufferObject);

	glEnableVertexArrayAttrib(VertexArrayObject, VAO_POSITION_LOCATION);
	glEnableVertexArrayAttrib(VertexArrayObject, VAO_COLOR_LOCATION);
	glEnableVertexArrayAttrib(VertexArrayObject, VAO_NORMAL_LOCATION);
	glEnableVertexArrayAttrib(VertexArrayObject, VAO_TEXTURECOORD_LOCATION);
	glEnableVertexArrayAttrib(VertexArrayObject, VAO_DRAWID_LOCATION);

	glVertexArrayAttribFormat(VertexArrayObject, VAO_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(VertexArrayObject, VAO_COLOR_LOCATION, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float));
	glVertexArrayAttribFormat(VertexArrayObject, VAO_NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float));
	glVertexArrayAttribFormat(VertexArrayObject, VAO_TEXTURECOORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float));
	glVertexArrayAttribIFormat(VertexArrayObject, VAO_DRAWID_LOCATION, 1, GL_UNSIGNED_INT, 0);

	glVertexArrayAttribBinding(VertexArrayObject, VAO_POSITION_LOCATION, 0);
	glVertexArrayAttribBinding(VertexArrayObject, VAO_COLOR_LOCATION, 0);
	glVertexArrayAttribBinding(VertexArrayObject, VAO_NORMAL_LOCATION, 0);
	glVertexArrayAttribBinding(VertexArrayObject, VAO_TEXTURECOORD_LOCATION, 0);
	glVertexArrayAttribBinding(VertexArrayObject, VAO_DRAWID_LOCATION, 1);

	glVertexArrayBindingDivisor(VertexArrayObject, 1, 1);

	// Draw command list buffer
	glGenBuffers(1, &DrawCommandObject);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, DrawCommandObject);
	glBufferStorage(GL_DRAW_INDIRECT_BUFFER, DrawList.CommandList.size() * sizeof(AEDrawElementsCommand), 0, flags);
	drawCommandPtr = glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, 0, DrawList.CommandList.size() * sizeof(AEDrawElementsCommand), flags);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);

	glObjectLabel(GL_BUFFER, VertexArrayObject, -1, "Vertex Array Buffer");
	glObjectLabel(GL_BUFFER, VertexBufferObject, -1, "Vertex Buffer");
	glObjectLabel(GL_BUFFER, IndicesBufferObject, -1, "Indices Buffer");
	glObjectLabel(GL_BUFFER, DrawIndexObject, -1, "Indirect Draw Buffer");
	glObjectLabel(GL_BUFFER, DrawCommandObject, -1, "Draw Command Buffer");
	
}

void AEEngine::CompileUBO()
{
	uint32_t flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	glGenBuffers(1, &GlobalParamsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, GlobalParamsUBO);
	glBufferStorage(GL_UNIFORM_BUFFER, sizeof(AEGlobalParameters), 0, flags);
	globalParamsPtr = glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(AEGlobalParameters), flags);
	glBindBufferBase(GL_UNIFORM_BUFFER, UBO_GLOBAL_PARAMS_LOCATION, GlobalParamsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AEEngine::CompileSSBO()
{
	uint32_t flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	glGenBuffers(1, &ModelMatrixSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ModelMatrixSSBO);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, DrawList.MatrixList.size() * sizeof(glm::mat4), 0, flags);
	modelMatrixPtr = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, DrawList.MatrixList.size() * sizeof(glm::mat4), flags);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SSBO_MODEL_MATRIX_LOCATION, ModelMatrixSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glObjectLabel(GL_BUFFER, ModelMatrixSSBO, -1, "Model Matrix SSBO");
}

void AEEngine::CopyData_GPU()
{
	// Unsafe if GPU is currently reading from data
	std::memcpy(vertexArrayPtr, DrawList.vertex_data, DrawList.vertex_count * DrawList.stride_size);
	std::memcpy(indicesArrayPtr, DrawList.indices_data, DrawList.indices_count * sizeof(uint32_t));
	std::memcpy(drawIndexesPtr, DrawList.IndexList.data(), DrawList.IndexList.size() * sizeof(uint32_t));
	std::memcpy(drawCommandPtr, DrawList.CommandList.data(), DrawList.CommandList.size() * sizeof(AEDrawElementsCommand));
	std::memcpy(modelMatrixPtr, DrawList.MatrixList.data(), DrawList.MatrixList.size() * sizeof(glm::mat4));
	std::memcpy(globalParamsPtr, &GlobalUBO, sizeof(AEGlobalParameters));
}

void AEEngine::UpdateUBO_GPU()
{
	std::memcpy(globalParamsPtr, &GlobalUBO, sizeof(AEGlobalParameters));
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
	glBindVertexArray(VertexArrayObject);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, DrawCommandObject);
}

void AEEngine::UnbindVAO()
{
	glBindVertexArray(0);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
}