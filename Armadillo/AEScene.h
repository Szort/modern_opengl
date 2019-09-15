#pragma once

#include <iostream>
#include "AECamera.h"
#include "AELight.h"
#include "AEPrimitive.h"
#include "AEGeometry.h"

class AEScene
{
	std::vector<glm::mat4>		Matrices;

	std::vector<AECamera>		Cameras;
	std::vector<AELight>		Lights;
	std::vector<AEPrimitive>	Primitives;
	std::vector<AEGeometry>		Geometry;

public:
	AEScene() {};
	~AEScene() {};

	void Add(AECamera& object)
	{
		Cameras.push_back(object);
		
		unsigned int matrixListSize = Matrices.size();
		Matrices.push_back(object.ModelMatrix);
		object.SetMatrixID(matrixListSize);
	};

	void Add(AELight& object)
	{
		Lights.push_back(object);

		unsigned int matrixListSize = Matrices.size();
		Matrices.push_back(object.ModelMatrix);
		object.SetMatrixID(matrixListSize);
	};

	void Add(AEGeometry& object)
	{
		Geometry.push_back(object);

		unsigned int matrixListSize = Matrices.size();
		Matrices.push_back(object.ModelMatrix);
		object.SetMatrixID(matrixListSize);
	};

	void Add(AEPrimitive& object)
	{
		Primitives.push_back(object);

		unsigned int matrixListSize = Matrices.size();
		Matrices.push_back(object.ModelMatrix);
		object.SetMatrixID(matrixListSize);
	};
};