#pragma once

#include "AEObject.h"

class AECamera : public AEObject
{
	float FieldOfView;
	float NearClip;
	float FarClip;

	glm::vec2 Resolution;
	float Ratio;

	glm::mat3 Model;
	glm::mat3 View;
	glm::mat3 Projection;

public:
	AECamera()
	{
		SetObjType(eAE_ObjectType_Camera);
	};
	~AECamera() {};


};