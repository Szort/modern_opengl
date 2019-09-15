#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "AEObject.h"

#define VAO_POSITION_LOCATION		0
#define	VAO_COLOR_LOCATION			1
#define VAO_NORMAL_LOCATION			2
#define VAO_TEXTURECOORD_LOCATION	3
#define VAO_DRAWID_LOCATION			4

struct AEDrawElementsCommand
{
	unsigned int vertexCount;
	unsigned int instanceCount;
	unsigned int firstIndex;
	unsigned int baseVertex;
	unsigned int baseInstance;
};

class AEGeometry : public AEObject
{
public:
	AEGeometry()
	{
		Position = glm::vec3(0.0f);
		ModelMatrix = glm::translate(ModelMatrix, Position);
		Init();
	};
	~AEGeometry() {};

	bool Init();
	void BindGeometry();
	void UnbindGeometry();
	glm::mat4 GetModelMatrix() { return ModelMatrix; };

	unsigned int vbo;
	unsigned int vao;
	unsigned int ibo;
	unsigned int iid;
	unsigned int indirectDrawBuffer;

	unsigned int mesh_count;
};