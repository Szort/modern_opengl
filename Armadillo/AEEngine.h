#pragma once

#include "AEObject.h"
#include "AEScene.h"

#define VAO_POSITION_LOCATION			0
#define	VAO_COLOR_LOCATION				1
#define VAO_NORMAL_LOCATION				2
#define VAO_TEXTURECOORD_LOCATION		3
#define VAO_DRAWID_LOCATION				4

#define SSBO_MODEL_MATRIX_LOCATION		0

#define UBO_GLOBAL_PARAMS_LOCATION		0

static uint32_t flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

struct AEVertexArrayPackeg
{
	aiVector3D	position;
	aiColor4D	color;
	aiVector3D	normal;
	aiVector3D	texCoord;
};

struct AEDrawElementsCommand
{
	unsigned int vertexCount;
	unsigned int instanceCount;
	unsigned int firstIndex;
	unsigned int baseVertex;
	unsigned int baseInstance;
};

struct AEDrawList
{
	// Packed data
	std::vector<AEVertexArrayPackeg>	vertex_data;
	std::vector<uint32_t>				indices_data;

	// Indirect draw command lists
	std::vector<AEDrawElementsCommand>	CommandList;
	std::vector<unsigned int>			IndexList;
	std::vector<glm::mat4>				MatrixList;
};

struct AEImportDataSlice
{
	std::vector<AEVertexArrayPackeg>	vertex_data;
	std::vector<uint32_t>				indices_data;
};

struct AEGlobalParameters
{
	glm::mat4				CameraVPMatrix;		// 64-byte
};

class AEEngine
{
	// Need to construct for static and dynamic objects
	uint32_t			VertexArrayObject;
	uint32_t			VertexBufferObject;
	uint32_t			IndicesBufferObject;
	uint32_t			DrawIndexObject;
	uint32_t			DrawCommandObject;
	void*				vertexArrayPtr;
	void*				indicesArrayPtr;
	void*				drawIndexesPtr;
	void*				drawCommandPtr;

	// All shader storage buffers need to be global
	uint32_t			ModelMatrixSSBO;
	void*				modelMatrixPtr;

	// All uniform buffers need to be global
	uint32_t			GlobalParamsUBO;
	void*				globalParamsPtr;

public:
	float				FpsCap;
	double				RenderTime;
	double				SleepTime;
	AEDrawList			DrawList;
	AEGlobalParameters	GlobalUBO;

	AEEngine(): FpsCap(60.0f), RenderTime(0), SleepTime(0) {};
	~AEEngine() {};

	void ConstructData(AEScene& scene);
	void CompileVAO();
	void CreateDrawCommandBuffer();
	void CreateVertexBuffer();
	void CreateUniformBuffer();
	void CreateShaderStorageBuffer();

	void CopyData_GPU();
	void UpdateUBO_GPU();

	void DrawGeometry();
	void DrawQuad();
	void BindVAO();
	void UnbindVAO();
	void Idle();
}; 