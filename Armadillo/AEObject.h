#pragma once

#include <vector>

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <string>

// Type of object list
enum AEObjectType
{
	eAE_ObjectType_Geometry,
	eAE_ObjectType_Primitive,
	eAE_ObjectType_Light,
	eAE_ObjectType_Camera,
	eAE_ObjectType_ReflectionProbe,
	eAE_ObjectType_Null
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
	// Data aligment
	unsigned int vert_data_size = 14;
	unsigned int stride_size = vert_data_size * sizeof(float);

	// Paced data
	float* vertex_data;
	unsigned int* indices_data;

	// Track data size
	unsigned int vertex_count;
	unsigned int indices_count;

	// Indirect draw command lists
	std::vector<AEDrawElementsCommand>	CommandList;
	std::vector<unsigned int>			IndexList;
	std::vector<glm::mat4>				MatrixList;
};

// Base hierarhical object
class AEObject
{
	AEObjectType	ObjectType;
	unsigned int	MatrixID;

public:
	glm::mat4		ModelMatrix;
	glm::vec3		Position;
	std::string		Name;
	bool			Selected;
	bool			Visible;

	AEObject() : ModelMatrix(glm::mat4(1.0f)) {};
	~AEObject() {};

	AEObjectType GetObjType() { return ObjectType; };

	void SetMatrixID(unsigned int& id) { MatrixID = id; };
	unsigned int GetMatrixID() { return MatrixID; };

protected:
	void SetObjType(const AEObjectType &type) { ObjectType = type; };
};