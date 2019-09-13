#pragma once

#include <vector>
#include <windows.h>

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

// Base hierarhical object
class AEObject
{
	AEObjectType ObjectType;

public:
	glm::vec3 Position;
	std::string Name;
	bool Selected;
	bool Visible;

	AEObject() {};
	~AEObject() {};
	AEObjectType GetObjType() { return ObjectType; };

protected:
	void SetObjType(const AEObjectType &type) { ObjectType = type; };
};