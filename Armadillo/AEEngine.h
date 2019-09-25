#pragma once

#include <vector>
#include "AEScene.h"
#include "AEObject.h"
#include "AEShader.h"
#include "AEDiagnostics.h"

#define VAO_POSITION_LOCATION			0
#define	VAO_COLOR_LOCATION				1
#define VAO_NORMAL_LOCATION				2
#define VAO_TEXTURECOORD_LOCATION		3
#define VAO_DRAWID_LOCATION				4

#define SSBO_MODEL_MATRIX_LOCATION		0

#define UBO_GLOBAL_PARAMS_LOCATION		0

struct AEImportDataSlice
{
	uint32_t	vertex_count;
	uint32_t	indices_count;

	float*			vertex_data;
	uint32_t*	indices_data;
	float*			vertex_data_start;
	uint32_t*	indices_data_start;
};

struct AEGlobalParameters
{
	glm::mat4	CameraVPMatrix;		// 64-byte
};

class AEEngine
{
	std::vector<AEShader>			Shaders;

	// Need to construct for static and dynamic objects
	uint32_t VertexArrayObject;
	uint32_t VertexBufferObject;
	uint32_t IndicesBufferObject;
	uint32_t DrawIndexObject;
	uint32_t DrawCommandObject;
	void* vertexArrayPtr;
	void* indicesArrayPtr;
	void* drawIndexesPtr;
	void* drawCommandPtr;

	// All shader storage buffers need to be global
	uint32_t ModelMatrixSSBO;
	void* modelMatrixPtr;

	// All uniform buffers need to be global
	uint32_t GlobalParamsUBO;
	void* globalParamsPtr;

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
	void CompileUBO();
	void CompileSSBO();
	void CopyData_GPU();
	void UpdateUBO_GPU();
	void BindVAO();
	void UnbindVAO();
	void Idle();
}; 