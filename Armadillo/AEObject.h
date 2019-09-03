#pragma once

#include <GLM/glm.hpp>
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
	glm::vec3 Position;
	std::string Name;
	AEObjectType ObjectType;

public:
	glm::vec3 GetPosition() { return Position; };
	void SetPosition(glm::vec3 &position) { Position = position; };
	std::string GetName() { return Name; };
	void SetName(const char* name) { Name = name; };
	AEObjectType GetObjType() { return ObjectType; };

protected:
	void SetObjType(const AEObjectType &type) { ObjectType = type; };
};