#include "AEEngine.h"

// Needed for Assimp import objects
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "AEDiagnostics.h"

void AEEngine::ConstructData(AEScene& scene)
{
	float							select_color = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
	float							color_offset = 1.61803398875f;
	DiagTimer						perf;
	Assimp::Importer				importer;
	std::vector<AEImportDataSlice>	ImportedData;

	uint32_t base_instance = 0;
	uint32_t vertex_count = 0;
	uint32_t indices_count = 0;

	uint32_t this_vertex_count;
	uint32_t this_indices_count;

	// Get all vertex data from engine objects
	if (scene.Primitives.size() != 0)
	{
		for (uint32_t object_id(0); object_id < scene.Primitives.size(); object_id++)
		{
			this_vertex_count = 0;
			this_indices_count = 0;

			uint32_t element_count = scene.Primitives[object_id].GetIndicesCount();

			AEDrawElementsCommand newDraw;
			newDraw.vertexCount = element_count;
			newDraw.instanceCount = 1;
			newDraw.firstIndex = indices_count;
			newDraw.baseVertex = vertex_count;
			newDraw.baseInstance = base_instance;

			DrawList.CommandList.push_back(newDraw);
			DrawList.IndexList.push_back(base_instance);

			AEObjectData objectData;
			objectData.Matrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));
			objectData.ColorID.r = ((base_instance & 0x000000FF) >> 0) / 255.0f;
			objectData.ColorID.g = ((base_instance & 0x0000FF00) >> 8) / 255.0f;
			objectData.ColorID.b = ((base_instance & 0x00FF0000) >> 16) / 255.0f;
			objectData.ColorID.a = 0.0f;
			DrawList.ObjectList.push_back(objectData);

			this_vertex_count += scene.Primitives[object_id].GetVertexCount();
			this_indices_count += element_count;

			vertex_count += scene.Primitives[object_id].GetVertexCount();
			indices_count += element_count;

			base_instance++;
		}

		for (uint32_t object_id(0); object_id < scene.Primitives.size(); object_id++)
		{
			AEImportDataSlice import_data;
			AEVertexArrayPackeg raw_data;

			for (uint32_t vert_id(0); vert_id < scene.Primitives[object_id].GetVertexCount(); vert_id++)
			{
				// Position: vec3
				raw_data.position = aiVector3D(
					scene.Primitives[object_id].GetVertexes()[5 * vert_id], 
					scene.Primitives[object_id].GetVertexes()[5 * vert_id + 1], 
					scene.Primitives[object_id].GetVertexes()[5 * vert_id + 2]);

				// Vertex Color: vec4
				raw_data.color = aiColor4D(0);

				// Vertex Normal: vec3
				raw_data.normal = aiVector3D(0);

				// Texture Coordinate: vec3
				raw_data.texCoord = aiVector3D(
					scene.Primitives[object_id].GetVertexes()[5 * vert_id + 3],
					scene.Primitives[object_id].GetVertexes()[5 * vert_id + 4],
					0.0f);

				// Push data to container
				import_data.vertex_data.push_back(raw_data);
			}

			// Pack indices geometry data
			for (uint32_t ind_id(0); ind_id < scene.Primitives[object_id].GetIndicesCount(); ind_id++)
			{
				import_data.indices_data.push_back(scene.Primitives[object_id].GetIndices()[ind_id]);
			}

			ImportedData.push_back(import_data);
		}
	}
	

	perf.StartTimer();

	// Import all assets in que
	for (uint32_t asset_id(0); asset_id < scene.AssetPaths.size(); asset_id++)
	{
		this_vertex_count = 0;
		this_indices_count = 0;

		// Import file and create pointer to scene
		const aiScene* import = importer.ReadFile(scene.AssetPaths[asset_id].c_str(), aiProcessPreset_TargetRealtime_Quality);

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
			std::cout << "No meshes imported from file: " << scene.AssetPaths[asset_id] << std::endl;
			continue;
		}

		AEImportDataSlice import_data;
		AEVertexArrayPackeg raw_data;

		// Check for mesh data in imported asset
		if (import->HasMeshes())
		{
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

				AEObjectData objectData;
				objectData.Matrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));
				objectData.ColorID.r = ((base_instance & 0x000000FF) >> 0) / 255.0f;
				objectData.ColorID.g = ((base_instance & 0x0000FF00) >> 8) / 255.0f;
				objectData.ColorID.b = ((base_instance & 0x00FF0000) >> 16) / 255.0f;
				objectData.ColorID.a = 0.0f;
				DrawList.ObjectList.push_back(objectData);

				this_vertex_count += import->mMeshes[i]->mNumVertices;
				this_indices_count += element_count;

				vertex_count += import->mMeshes[i]->mNumVertices;
				indices_count += element_count;

				base_instance++;
			}

			// Allocate memory for array packing
			import_data.vertex_data.reserve(this_vertex_count);
			import_data.indices_data.reserve(this_indices_count);

			// Pack vertex geometry data
			for (uint32_t i(0); i < import->mNumMeshes; i++)
			{
				bool b_vertexColor = import->mMeshes[i]->HasVertexColors(0);
				bool b_textureCoord = import->mMeshes[i]->HasTextureCoords(0);
				uint32_t vert_count = import->mMeshes[i]->mNumVertices;
				uint32_t face_count = import->mMeshes[i]->mNumFaces;

				for (uint32_t vert_id(0); vert_id < vert_count; vert_id++)
				{
					// Position: vec3
					raw_data.position = import->mMeshes[i]->mVertices[vert_id];

					// Vertex Color: vec4
					if (b_vertexColor) raw_data.color = *import->mMeshes[i]->mColors[0];

					// Vertex Normal: vec3
					raw_data.normal = import->mMeshes[i]->mNormals[vert_id];

					// Texture Coordinate: vec3
					if (b_textureCoord) raw_data.texCoord = *import->mMeshes[i]->mTextureCoords[0];

					// Push data to container
					import_data.vertex_data.push_back(raw_data);
				}

				// Pack indices geometry data
				for (uint32_t face_id(0); face_id < face_count; face_id++)
				{
					import_data.indices_data.push_back(import->mMeshes[i]->mFaces[face_id].mIndices[0]);
					import_data.indices_data.push_back(import->mMeshes[i]->mFaces[face_id].mIndices[1]);
					import_data.indices_data.push_back(import->mMeshes[i]->mFaces[face_id].mIndices[2]);
				}
			}
		}

		// Materials data
		//if (import->HasMaterials())
		//{
		//	uint32_t material_count = import->mNumMaterials;
		//	for (uint32_t idx(0); idx < material_count; idx++)
		//	{
		//		aiString material_name = import->mMaterials[idx]->GetName();
		//	}
		//}
		//
		//// Texture data
		//if (import->HasTextures())
		//{
		//	//import->
		//}

		// Store imported slice
		ImportedData.push_back(import_data);
	}

	DrawList.vertex_data.reserve(vertex_count);
	DrawList.indices_data.reserve(indices_count);

	// Pack all data
	for (uint32_t id(0); id < ImportedData.size(); id++)
	{
		for (uint32_t v_id(0); v_id < ImportedData[id].vertex_data.size(); v_id++)
			DrawList.vertex_data.push_back(ImportedData[id].vertex_data[v_id]);
	
		for (uint32_t i_id(0); i_id < ImportedData[id].indices_data.size(); i_id++)
			DrawList.indices_data.push_back(ImportedData[id].indices_data[i_id]);
	}

	std::cout << "Time packing: " << perf.GetTimer() << "ns" << std::endl;

	CreateDrawCommandBuffer();
	CreateVertexBuffer();
	CreateUniformBuffer();
	CreateShaderStorageBuffer();
}

void AEEngine::CompileVAO()
{
	// Generate geometry buffers
	glCreateVertexArrays(1, &VertexArrayObject);
	glCreateBuffers(1, &VertexBufferObject);
	glCreateBuffers(1, &IndicesBufferObject);
	glCreateBuffers(1, &DrawIndexObject);

	glVertexArrayVertexBuffer(VertexArrayObject, 0, VertexBufferObject, 0, sizeof(AEVertexArrayPackeg));
	glVertexArrayVertexBuffer(VertexArrayObject, 1, DrawIndexObject, 0, sizeof(uint32_t));
	glVertexArrayElementBuffer(VertexArrayObject, IndicesBufferObject);

	glEnableVertexArrayAttrib(VertexArrayObject, VAO_POSITION_LOCATION);
	glEnableVertexArrayAttrib(VertexArrayObject, VAO_COLOR_LOCATION);
	glEnableVertexArrayAttrib(VertexArrayObject, VAO_NORMAL_LOCATION);
	glEnableVertexArrayAttrib(VertexArrayObject, VAO_TEXTURECOORD_LOCATION);
	glEnableVertexArrayAttrib(VertexArrayObject, VAO_DRAWID_LOCATION);

	glVertexArrayAttribFormat(VertexArrayObject, VAO_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(VertexArrayObject, VAO_COLOR_LOCATION, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
	glVertexArrayAttribFormat(VertexArrayObject, VAO_NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float));
	glVertexArrayAttribFormat(VertexArrayObject, VAO_TEXTURECOORD_LOCATION, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float));
	glVertexArrayAttribIFormat(VertexArrayObject, VAO_DRAWID_LOCATION, 1, GL_UNSIGNED_INT, 0);

	glVertexArrayAttribBinding(VertexArrayObject, VAO_POSITION_LOCATION, 0);
	glVertexArrayAttribBinding(VertexArrayObject, VAO_COLOR_LOCATION, 0);
	glVertexArrayAttribBinding(VertexArrayObject, VAO_NORMAL_LOCATION, 0);
	glVertexArrayAttribBinding(VertexArrayObject, VAO_TEXTURECOORD_LOCATION, 0);
	glVertexArrayAttribBinding(VertexArrayObject, VAO_DRAWID_LOCATION, 1);

	glVertexArrayBindingDivisor(VertexArrayObject, 1, 1);

	glObjectLabel(GL_BUFFER, VertexArrayObject, -1, "Vertex Array Buffer");
	glObjectLabel(GL_BUFFER, VertexBufferObject, -1, "Vertex Buffer");
	glObjectLabel(GL_BUFFER, IndicesBufferObject, -1, "Indices Buffer");
	glObjectLabel(GL_BUFFER, DrawIndexObject, -1, "Indirect Draw Buffer");
}

void AEEngine::CreateDrawCommandBuffer()
{
	// Draw command list buffer
	glCreateBuffers(1, &DrawCommandObject);
	glNamedBufferStorage(DrawCommandObject, DrawList.CommandList.size() * sizeof(AEDrawElementsCommand), 0, flags);
	drawCommandPtr = glMapNamedBufferRange(DrawCommandObject, 0, DrawList.CommandList.size() * sizeof(AEDrawElementsCommand), flags);

	glObjectLabel(GL_BUFFER, DrawCommandObject, -1, "Draw Command Buffer");
}

void AEEngine::CreateVertexBuffer()
{
	glNamedBufferStorage(VertexBufferObject, DrawList.vertex_data.size() * sizeof(AEVertexArrayPackeg), 0, flags);
	vertexArrayPtr = glMapNamedBufferRange(VertexBufferObject, 0, DrawList.vertex_data.size() * sizeof(AEVertexArrayPackeg), flags);

	glNamedBufferStorage(IndicesBufferObject, DrawList.indices_data.size() * sizeof(uint32_t), 0, flags);
	indicesArrayPtr = glMapNamedBufferRange(IndicesBufferObject, 0, DrawList.indices_data.size() * sizeof(uint32_t), flags);

	glNamedBufferStorage(DrawIndexObject, DrawList.IndexList.size() * sizeof(uint32_t), 0, flags);
	drawIndexesPtr = glMapNamedBufferRange(DrawIndexObject, 0, DrawList.IndexList.size() * sizeof(uint32_t), flags);
}

void AEEngine::CreateUniformBuffer()
{
	// No direct state access functionality for UBO but we map buffer in perssistent way.
	glGenBuffers(1, &GlobalParamsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, GlobalParamsUBO);
	glBufferStorage(GL_UNIFORM_BUFFER, sizeof(AEGlobalParameters), 0, flags);
	globalParamsPtr = glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(AEGlobalParameters), flags);
	glBindBufferBase(GL_UNIFORM_BUFFER, UBO_GLOBAL_PARAMS_LOCATION, GlobalParamsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glObjectLabel(GL_BUFFER, GlobalParamsUBO, -1, "Global Parameters Buffer");
}

void AEEngine::CreateShaderStorageBuffer()
{
	if (ObjectDataSSBO != 0)
	{
		glUnmapNamedBuffer(ObjectDataSSBO);
		glDeleteBuffers(1, &ObjectDataSSBO);
	}

	// No direct state access functionality for SSBO but we map buffer in perssistent way.
	glGenBuffers(1, &ObjectDataSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ObjectDataSSBO);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, DrawList.ObjectList.size() * sizeof(AEObjectData), 0, flags);
	objectDataPtr = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, DrawList.ObjectList.size() * sizeof(AEObjectData), flags);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SSBO_MODEL_MATRIX_LOCATION, ObjectDataSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glObjectLabel(GL_BUFFER, ObjectDataSSBO, -1, "Object Data SSBO");
}

void AEEngine::CopyData_GPU()
{
	// Unsafe if GPU is currently reading from data
	// but we are moving data before main rendering loop.
	std::memcpy(vertexArrayPtr, DrawList.vertex_data.data(), DrawList.vertex_data.size() * sizeof(AEVertexArrayPackeg));
	std::memcpy(indicesArrayPtr, DrawList.indices_data.data(), DrawList.indices_data.size() * sizeof(uint32_t));
	std::memcpy(drawIndexesPtr, DrawList.IndexList.data(), DrawList.IndexList.size() * sizeof(uint32_t));
	std::memcpy(drawCommandPtr, DrawList.CommandList.data(), DrawList.CommandList.size() * sizeof(AEDrawElementsCommand));
	std::memcpy(objectDataPtr, DrawList.ObjectList.data(), DrawList.ObjectList.size() * sizeof(AEObjectData));
	std::memcpy(globalParamsPtr, &GlobalUBO, sizeof(AEGlobalParameters));
}

void AEEngine::UpdateUBO_GPU()
{
	// TODO:
	// Need to do proper syncing. This is unsafe.
	std::memcpy(globalParamsPtr, &GlobalUBO, sizeof(AEGlobalParameters));
}

void AEEngine::Idle()
{
	// Sleeping time to not waste power.
	SleepTime = (1000 / FpsCap) - RenderTime;
	SleepTime = SleepTime < 0 ? 0 : SleepTime;
	Sleep(SleepTime);
}

void AEEngine::DrawGeometry()
{
	glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, (uint32_t)DrawList.CommandList.size(), 0);
}

void AEEngine::DrawQuad()
{
	// Works only if quad is first in draw command list
	glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, 1, 0);
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