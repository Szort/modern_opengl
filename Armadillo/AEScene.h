#pragma once

#include <list>

#include "AECamera.h"
#include "AELight.h"
#include "AEPrimitive.h"
#include "AEMesh.h"

class AEScene
{
public:
	std::vector<std::string>	AssetPaths;
	std::vector<AECamera>		Cameras;
	std::vector<AELight>		Lights;
	std::vector<AEPrimitive>	Primitives;
	std::vector<AEMesh>			Meshes;

	AEScene() {};
	~AEScene() {};

	void ImportAsset(std::string objFiled);

	void Add(AECamera& object)
	{
		Cameras.push_back(object);
	};

	void Add(AELight& object)
	{
		Lights.push_back(object);
	};

	void Add(AEMesh& object)
	{
		Meshes.push_back(object);
	};

	void Add(AEPrimitive& object)
	{
		Primitives.push_back(object);
	};
};