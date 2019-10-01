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