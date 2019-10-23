#pragma once

#include "AECore.h"
#include "AETexture.h"

class AEMaterial
{
	glm::vec4	AlbedoValue;
	glm::vec3	NormalValue;
	float		RoughnessValue;
	float		MetalnessValue;
	
	AETexture	AlbedoTexture;
	AETexture	NormalTexture;
	AETexture	RoughnessTexture;
	AETexture	MetalnessTexture;

public:
	AEMaterial() {};
	~AEMaterial() {};
};