#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <assimp/scene.h>

#include "AEObject.h"

class AEMesh : public AEObject
{
	aiMesh* Mesh;

public:
	AEMesh()
	{
		Position = glm::vec3(0.0f);
		ModelMatrix = glm::translate(ModelMatrix, Position);
	};
	~AEMesh() {};

	void SetMesh(aiMesh* mesh) { Mesh = mesh; };
	aiMesh* GetMesh() { return Mesh; };
};