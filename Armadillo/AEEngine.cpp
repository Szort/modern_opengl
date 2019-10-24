#include "AEEngine.h"

// Needed for Assimp import objects
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "AEDiagnostics.h"

float				AEEngine::FpsCap = 60.0f;
double				AEEngine::RenderTime = 0;
double				AEEngine::SleepTime = 0;
bool				AEEngine::DebugBBox = false;
int					AEEngine::SelectedID = 0;
AEGlobalParameters	AEEngine::GlobalUBO;

void AEEngine::ConstructData(AEScene& scene)
{
	DiagTimer						perf;
	Assimp::Importer				importer;
	std::vector<AEImportDataSlice>	importedData;

	uint32_t this_vertex_count;
	uint32_t this_indices_count;	

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
			AddToDrawCommand(scene, import, this_vertex_count, this_indices_count);

			// Allocate memory for array packing
			import_data.VertexData.reserve(this_vertex_count);
			import_data.IndicesData.reserve(this_indices_count);

			// Pack vertex geometry data
			for (uint32_t i(0); i < import->mNumMeshes; i++)
			{
				bool b_vertexColor = import->mMeshes[i]->HasVertexColors(0);
				bool b_textureCoord = import->mMeshes[i]->HasTextureCoords(0);
				uint32_t vert_count = import->mMeshes[i]->mNumVertices;
				uint32_t face_count = import->mMeshes[i]->mNumFaces;

				AEObjectMinMax minmax;
				minmax.x_max = import->mMeshes[i]->mVertices[0].x;
				minmax.x_min = minmax.x_max;
				minmax.y_max = import->mMeshes[i]->mVertices[0].y;
				minmax.y_min = minmax.y_max;
				minmax.z_max = import->mMeshes[i]->mVertices[0].z;
				minmax.z_min = minmax.z_max;

				for (uint32_t vert_id(0); vert_id < vert_count; vert_id++)
				{
					// Position: vec3
					raw_data.position = import->mMeshes[i]->mVertices[vert_id];

					// Min max calculations
					minmax.x_min = minmax.x_min > raw_data.position.x ? raw_data.position.x : minmax.x_min;
					minmax.x_max = minmax.x_max < raw_data.position.x ? raw_data.position.x : minmax.x_max;
					minmax.y_min = minmax.y_min > raw_data.position.y ? raw_data.position.y : minmax.y_min;
					minmax.y_max = minmax.y_max < raw_data.position.y ? raw_data.position.y : minmax.y_max;
					minmax.z_min = minmax.z_min > raw_data.position.z ? raw_data.position.z : minmax.z_min;
					minmax.z_max = minmax.z_max < raw_data.position.z ? raw_data.position.z : minmax.z_max;

					// Vertex Color: vec4
					if (b_vertexColor) raw_data.color = *import->mMeshes[i]->mColors[0];

					// Vertex Normal: vec3
					raw_data.normal = import->mMeshes[i]->mNormals[vert_id];

					// Texture Coordinate: vec3
					if (b_textureCoord) raw_data.texCoord = *import->mMeshes[i]->mTextureCoords[0];

					// Push data to container
					import_data.VertexData.push_back(raw_data);
				}

				// Pack indices geometry data
				for (uint32_t face_id(0); face_id < face_count; face_id++)
				{
					import_data.IndicesData.push_back(import->mMeshes[i]->mFaces[face_id].mIndices[0]);
					import_data.IndicesData.push_back(import->mMeshes[i]->mFaces[face_id].mIndices[1]);
					import_data.IndicesData.push_back(import->mMeshes[i]->mFaces[face_id].mIndices[2]);
				}

				scene.Meshes[i].SetBoundBox(minmax);
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
		importedData.push_back(import_data);
	}

	// Get all vertex data from engine objects
	if (scene.Primitives.size() != 0)
	{
		AddToDrawCommand(scene, this_vertex_count, this_indices_count);

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
				import_data.VertexData.push_back(raw_data);
			}

			// Pack indices geometry data
			for (uint32_t ind_id(0); ind_id < scene.Primitives[object_id].GetIndicesCount(); ind_id++)
			{
				import_data.IndicesData.push_back(scene.Primitives[object_id].GetIndices()[ind_id]);
			}

			importedData.push_back(import_data);
		}
	}

	for (uint32_t i(0); i < DrawList.ObjectList.size(); i++)
		DrawList.ObjectList[i].BBox = scene.Meshes[i].GetBoundBox();

	DrawList.VertexData.reserve(DrawList.VertexCount);
	DrawList.IndicesData.reserve(DrawList.IndicesCount);

	// Pack all data
	for (uint32_t id(0); id < importedData.size(); id++)
	{
		for (uint32_t v_id(0); v_id < importedData[id].VertexData.size(); v_id++)
			DrawList.VertexData.push_back(importedData[id].VertexData[v_id]);
	
		for (uint32_t i_id(0); i_id < importedData[id].IndicesData.size(); i_id++)
			DrawList.IndicesData.push_back(importedData[id].IndicesData[i_id]);
	}

	std::cout << "Time packing: " << perf.GetTimer() << "ns" << std::endl;

	CreateDrawCommandBuffer();
	CreateVertexBuffer();
	CreateUniformBuffer();
	CreateShaderStorageBuffer();
}

void AEEngine::AddToDrawCommand(AEScene& eng_scene, const aiScene* imp_scene, uint32_t& vert_count, uint32_t& ind_count)
{
	for (uint32_t i(0); i < imp_scene->mNumMeshes; i++)
	{
		AEMesh mesh_engine_data;
		mesh_engine_data.SetName(imp_scene->mMeshes[i]->mName.C_Str());
		mesh_engine_data.ElementCount = imp_scene->mMeshes[i]->mNumFaces * imp_scene->mMeshes[i]->mFaces[0].mNumIndices;
		mesh_engine_data.VertexCount = imp_scene->mMeshes[i]->mNumVertices;

		MakeDrawCommand(mesh_engine_data, vert_count, ind_count);
		eng_scene.Add(mesh_engine_data);
	}
}

void AEEngine::AddToDrawCommand(AEScene& eng_scene, uint32_t& vert_count, uint32_t& ind_count)
{
	for (uint32_t object_id(0); object_id < eng_scene.Primitives.size(); object_id++)
	{
		vert_count = 0;
		ind_count = 0;

		AEMesh mesh_engine_data;
		mesh_engine_data.SetName("Primitive");
		mesh_engine_data.ElementCount = eng_scene.Primitives[object_id].GetIndicesCount();
		mesh_engine_data.VertexCount = eng_scene.Primitives[object_id].GetVertexCount();

		MakeDrawCommand(mesh_engine_data, vert_count, ind_count);
		eng_scene.Add(mesh_engine_data);
	}
}

void AEEngine::MakeDrawCommand(AEMesh& mesh, uint32_t& vert_count, uint32_t& ind_count)
{
	AEDrawObjectsCommand newDraw;
	newDraw.vertexCount = mesh.ElementCount;
	newDraw.instanceCount = 1;
	newDraw.firstIndex = DrawList.IndicesCount;
	newDraw.baseVertex = DrawList.VertexCount;
	newDraw.baseInstance = DrawList.BaseInstance;

	mesh.SetDrawCommand(newDraw);
	DrawList.CommandList.push_back(newDraw);
	DrawList.IndexList.push_back(DrawList.BaseInstance);

	AEObjectData objectData;
	objectData.Matrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));
	DrawList.ObjectList.push_back(objectData);

	vert_count += mesh.VertexCount;
	ind_count += mesh.ElementCount;

	DrawList.VertexCount += mesh.VertexCount;
	DrawList.IndicesCount += mesh.ElementCount;

	DrawList.BaseInstance++;
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
	glNamedBufferStorage(DrawCommandObject, DrawList.CommandList.size() * sizeof(AEDrawObjectsCommand), 0, flags);
	drawCommandPtr = glMapNamedBufferRange(DrawCommandObject, 0, DrawList.CommandList.size() * sizeof(AEDrawObjectsCommand), flags);

	glObjectLabel(GL_BUFFER, DrawCommandObject, -1, "Draw Command Buffer");
}

void AEEngine::CreateVertexBuffer()
{
	glNamedBufferStorage(VertexBufferObject, DrawList.VertexData.size() * sizeof(AEVertexArrayPackeg), 0, flags);
	vertexArrayPtr = glMapNamedBufferRange(VertexBufferObject, 0, DrawList.VertexData.size() * sizeof(AEVertexArrayPackeg), flags);

	glNamedBufferStorage(IndicesBufferObject, DrawList.IndicesData.size() * sizeof(uint32_t), 0, flags);
	indicesArrayPtr = glMapNamedBufferRange(IndicesBufferObject, 0, DrawList.IndicesData.size() * sizeof(uint32_t), flags);

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
	std::memcpy(vertexArrayPtr, DrawList.VertexData.data(), DrawList.VertexData.size() * sizeof(AEVertexArrayPackeg));
	std::memcpy(indicesArrayPtr, DrawList.IndicesData.data(), DrawList.IndicesData.size() * sizeof(uint32_t));
	std::memcpy(drawIndexesPtr, DrawList.IndexList.data(), DrawList.IndexList.size() * sizeof(uint32_t));
	std::memcpy(drawCommandPtr, DrawList.CommandList.data(), DrawList.CommandList.size() * sizeof(AEDrawObjectsCommand));
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
	glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, (uint32_t)DrawList.CommandList.size() - 1, 0);
}

void AEEngine::DrawBoundingBox()
{
	glDrawArraysInstanced(GL_POINTS, 0, 1, (uint32_t)DrawList.ObjectList.size());
}

void AEEngine::DrawSelected()
{
	glDrawElementsBaseVertex(
		GL_TRIANGLES,
		DrawList.CommandList[SelectedID].vertexCount,
		GL_UNSIGNED_INT,
		(void*)(DrawList.CommandList[SelectedID].firstIndex * sizeof(uint32_t)),
		DrawList.CommandList[SelectedID].baseVertex);
}

void AEEngine::DrawQuad()
{
	glDrawElementsBaseVertex(
		GL_TRIANGLES,
		DrawList.CommandList[(uint32_t)DrawList.ObjectList.size() - 1].vertexCount,
		GL_UNSIGNED_INT,
		(void*)(DrawList.CommandList[(uint32_t)DrawList.ObjectList.size() - 1].firstIndex * sizeof(uint32_t)),
		DrawList.CommandList[(uint32_t)DrawList.ObjectList.size() - 1].baseVertex);
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