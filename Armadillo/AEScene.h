#pragma once

#include "AECamera.h"
#include "AELight.h"
#include "AEPrimitive.h"
#include "AEGeometry.h"

class AEScene
{
	std::vector<AECamera>		Cameras;
	std::vector<AELight>		Lights;
	std::vector<AEPrimitive>	Primitives;
	std::vector<AEGeometry>		Geometry;

public:
	AEScene() {};
	~AEScene() {};

	template <class T>
	void Add(T object);

	template <class T>
	void Destroy(T list);
};