#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

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

struct AEObjectMinMax
{
	float x_min;
	float x_max;
	float y_min;
	float y_max;
	float z_min;
	float z_max;

	AEObjectMinMax() {
		x_min = 0.0f;
		x_max = 0.0f;
		y_min = 0.0f;
		y_max = 0.0f;
		z_min = 0.0f;
		z_max = 0.0f;
	};
};

struct AEObjectData
{
	AEObjectMinMax	BBox;
	glm::vec2		Padding0;
	glm::mat4		Matrix;
};

struct AEDrawObjectsCommand
{
	uint32_t vertexCount;
	uint32_t instanceCount;
	uint32_t firstIndex;
	uint32_t baseVertex;
	uint32_t baseInstance;

	AEDrawObjectsCommand()
	{
		vertexCount = 0;
		instanceCount = 0;
		firstIndex = 0;
		baseVertex = 0;
		baseInstance = 0;
	};
};

// Base hierarhical object
class AEObject
{
	AEObjectType	ObjectType;
	std::string		Name;
	uint32_t		MatrixID;

public:
	glm::mat4		ModelMatrix;
	glm::vec3		Position;
	bool			Selected;
	bool			Visible;

	AEObject() : 
		ModelMatrix(glm::mat4(1.0f)), 
		Position(glm::vec3(0.0f)) {};
	virtual ~AEObject() {};

	AEObjectType GetObjType() { return ObjectType; };

	void SetMatrixID(unsigned int& id)	{ MatrixID = id; };
	void SetName(std::string name)		{ Name = name; };

	uint32_t GetMatrixID() const		{ return MatrixID; };
	std::string GetName() const			{ return Name; };

protected:
	void SetObjType(const AEObjectType &type) { ObjectType = type; };
};