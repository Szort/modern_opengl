#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <assimp/scene.h>

#include "AEObject.h"

class AEMesh : public AEObject
{
	glm::vec4	BoundBoxMinMax;
	uint32_t	VertexCount;
	uint32_t	InstanceCount;
	uint32_t	FirstIndex;
	uint32_t	BaseVertex;
	uint32_t	BaseInstance;

public:
	AEMesh() {};
	~AEMesh() {};

	void SetPosition(glm::vec3 new_pos) { ModelMatrix = glm::translate(ModelMatrix, new_pos); };
	void SetBoundBox(glm::vec4 bbox) { BoundBoxMinMax = bbox; };

	glm::vec3 GetPosition() { return Position; };
	glm::vec4 GetBoundBox() const { return BoundBoxMinMax; };

};