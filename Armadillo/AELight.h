#pragma once

#include <GLM/glm.hpp>

#include "AEObject.h"

// Light falloff type
enum AEFalloffType
{
	eAE_Falloff_Linear,
	eAE_Falloff_Cubic,
	eAE_Falloff_Quadratic
};

// Base light component
class AELight : public AEObject
{
	glm::vec3 Color;
	float Intensity;
	bool CastLight;
	bool CastShadow;
	AEFalloffType FalloffType;

public:
	AELight() :
		Color(glm::vec3(0.0)),
		Intensity(1.0),
		CastLight(true),
		CastShadow(true),
		FalloffType(eAE_Falloff_Linear)
	{};
	~AELight() {};

	AELight(glm::vec3 &position) { Position = position; };
	AELight(float &intensity) { Intensity = intensity; };

	glm::vec3 GetColor() { return Color; };
	void SetColor(glm::vec3 &color) { Color = color; };
	float GetIntensity() { return Intensity; };
	void SetIntensity(float &intensity) { Intensity = intensity; };
};

// Point light
class AEPointLight : public AELight
{
	float Range;

public:
	AEPointLight() :
		Range(10.0)
	{
		SetObjType(eAE_ObjectType_Light);
	};
	~AEPointLight() {};

	float GetRange() { return Range; };
	void SetRange(float &range) { Range = range; };
};

// Directional light
class AEDirectionalLight : public AELight
{
	glm::vec3 Direction;

public:
	AEDirectionalLight() :
		Direction(10.0)
	{
		SetObjType(eAE_ObjectType_Light);
	};
	~AEDirectionalLight() {};

	glm::vec3 GetDirection() { return Direction; };
	void SetDirection(glm::vec3 &direction) { Direction = direction; };
};