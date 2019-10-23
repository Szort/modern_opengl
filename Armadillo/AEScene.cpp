#include "AEScene.h"

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