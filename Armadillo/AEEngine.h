#pragma once

#include "AEObject.h"
#include "AEScene.h"
#include "AEFrameBuffer.h"

struct AEVertexArrayPackeg
{
	aiVector3D	position;
	aiColor4D	color;
	aiVector3D	normal;
	aiVector3D	texCoord;
};

struct AEDrawList
{
	uint32_t							VertexCount;
	uint32_t							IndicesCount;
	uint32_t							BaseInstance;

	// Packed data
	std::vector<AEVertexArrayPackeg>	VertexData;
	std::vector<uint32_t>				IndicesData;

	// Indirect draw command lists
	std::vector<AEDrawObjectsCommand>	CommandList;
	std::vector<AEObjectData>			ObjectList;
	std::vector<uint32_t>				IndexList;

	AEDrawList() :
		VertexCount(0),
		IndicesCount(0),
		BaseInstance(0)
	{};
};

struct AEImportDataSlice
{
	std::vector<AEVertexArrayPackeg>	VertexData;
	std::vector<uint32_t>				IndicesData;
};

struct AEGlobalParameters
{
	glm::mat4	CameraVPMatrix;
	glm::mat4	CameraPMatrix_Inv;
	glm::vec3	AmbientColor;
	float		padding0;

	AEGlobalParameters() :
		CameraVPMatrix(glm::mat4(1.0f)),
		CameraPMatrix_Inv(glm::mat4(1.0f)),
		AmbientColor(glm::vec3(0.0f))
	{};
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

	// All uniform buffers need to be global
	AEShaderBuffer				Global_UBO;
	AEShaderBuffer				Objects_SSBO;

public:
	static float				FpsCap;
	static double				RenderTime;
	static double				SleepTime;
	static bool					DebugBBox;
	static int					SelectedID;
	static AEGlobalParameters	GlobalUBO;

	AEDrawList					DrawList;

	AEEngine() {};
	~AEEngine() {};

	void ConstructData(AEScene& scene);
	void CompileVAO();
	void CreateDrawCommandBuffer();
	void CreateVertexBuffer();

	void CopyData_GPU();
	void UpdateUBO_GPU();

	void AddToDrawCommand(AEScene& eng_scene, const aiScene* imp_scene, uint32_t& vert_count, uint32_t& ind_count);
	void AddToDrawCommand(AEScene& eng_scene, uint32_t& vert_count, uint32_t& ind_count);
	void MakeDrawCommand(AEMesh& mesh, uint32_t& vert_count, uint32_t& ind_count);
	void DrawGeometry();
	void DrawSelected();
	void DrawBoundingBox();
	void DrawQuad();
	void BindVAO();
	void UnbindVAO();
	void Idle();
}; 