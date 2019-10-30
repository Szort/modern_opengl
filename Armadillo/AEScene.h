#pragma once

#include "AECamera.h"
#include "AELight.h"
#include "AEPrimitive.h"
#include "AEMesh.h"
#include "AEMaterial.h"
#include "AEShaderBuffer.h"


class AEScene
{
	// All shader storage buffers need to be global
	AEShaderBuffer				Object_SSBO;
	AEShaderBuffer				Light_SSBO;
	AEShaderBuffer				Material_SSBO;

public:
	std::vector<std::string>	AssetPaths;
	std::vector<AECamera>		Cameras;
	std::vector<AELight>		Lights;
	std::vector<AEPrimitive>	Primitives;
	std::vector<AEMesh>			Meshes;
	std::vector<AEMaterial>		Materials;

	AEScene() {};
	~AEScene() {};

	void ImportAsset(std::string objFiled);
	void ConstructBuffers();

	void Add(AECamera& object);
	void Add(AELight& object);
	void Add(AEPrimitive& object);
	void Add(AEMesh& object);
	void Add(AEMaterial& object);
};