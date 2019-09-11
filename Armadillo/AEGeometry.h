#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "AEObject.h"

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
};