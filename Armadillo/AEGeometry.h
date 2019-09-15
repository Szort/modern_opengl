#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "AEObject.h"

class AEGeometry : public AEObject
{
public:
	AEGeometry()
	{
		Position = glm::vec3(0.0f);
		ModelMatrix = glm::translate(ModelMatrix, Position);
	};
	~AEGeometry() {};

	bool Import(std::string objFile, AEDrawList& drawList);
	glm::mat4 GetModelMatrix() { return ModelMatrix; };
};