#include "AEScene.h"

void AEScene::Add(AECamera& object)
{
	Cameras.push_back(object);
};

void AEScene::Add(AELight& object)
{
	Lights.push_back(object);
};

void AEScene::Add(AEPrimitive& object)
{
	Primitives.push_back(object);
};

void AEScene::Add(AEMesh& object)
{
	Meshes.push_back(object);
};

void AEScene::Add(AEMaterial& object)
{
	Materials.push_back(object);
};

void AEScene::ImportAsset(std::string objFile)
{
	// Determin if file exist
	std::ifstream fin(objFile.c_str());
	if (!fin.fail()) {
		fin.close();

		// Push to list of existing file if file exist
		AssetPaths.push_back(objFile);
	}
	else {
		std::cout << "No import!" << std::endl;
		std::cin.get();
	}
}

void AEScene::ConstructBuffers()
{
	std::vector<AELightData> light_data;
	light_data.reserve(Lights.size());
	for (auto light : Lights)
	{
		light_data.push_back(light.GetLightData());
	}

	Light_SSBO.CreateBuffer(eAE_ShaderBufferType_SSBO, SSBO_LIGHTS_LOCATION, light_data.size() * sizeof(AELightData), "Light Data SSBO");
	Light_SSBO.CopyDataToGPU(light_data.data());
}