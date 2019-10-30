#pragma once

#include "AEObject.h"

// Light falloff type
enum AELightType
{
	eAE_LightType_Directional,
	eAE_LightType_Point,
	eAE_LightType_Spot,
	eAE_LightType_Area
};

struct AELightData
{

	glm::vec3	Position;
	float		Radius;
	glm::vec3	Color;
	float		Intensity;
	glm::vec3	Direction;

	AELightData() :
		Position(glm::vec3(0.0f)),
		Radius(10.0),
		Color(glm::vec3(0.0f)),
		Intensity(1.0),
		Direction(glm::vec3(0.0f))
	{};

	AELightData(glm::vec3 *position)
	{
		Position = *position;
		Radius = 10.0f,
		Color = glm::vec3(0.0f);
		Intensity = 1.0f;
		Direction = glm::vec3(0.0f);
	};

	~AELightData() {};
};

// Base light component
class AELight : public AEObject
{
	bool			CastLight;
	bool			CastShadow;
	AELightData		Data;
	AELightType		LightType;

public:
	AELight() :
		Data(&Position),
		CastLight(true),
		CastShadow(true),
		LightType(eAE_LightType_Point)
	{};

	AELight(AELightType type) 
	{
		Data.Position = Position;
		CastLight = true;
		CastShadow = true;
		LightType = type;
	}

	~AELight() {};

	AELight(glm::vec3 &position) { Data.Position = position; };
	AELight(float &intensity) { Data.Intensity = intensity; };

	glm::vec3 GetColor() { return Data.Color; };
	float GetIntensity() { return Data.Intensity; };
	float GetRange() { return Data.Radius; };
	glm::vec3 GetDirection() { return Data.Direction; };
	AELightData& GetLightData() { return Data; };

	void SetPosition(glm::vec3 position) { Data.Position = position; };
	void SetColor(glm::vec3 color)	{ Data.Color = color; };
	void SetIntensity(float intensity) { Data.Intensity = intensity; };
	void SetRange(float radius) { Data.Radius = radius; };
	void SetDirection(glm::vec3 direction) { Data.Direction = direction; };
};