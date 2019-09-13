#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "AEObject.h"

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
	glm::mat4 ModelMatrix;

public:
	AEGeometry(): ModelMatrix(glm::mat4(1.0f))
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
};