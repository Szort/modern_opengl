#include <fstream>
#include <iostream>

#include "AEScene.h"

bool AEScene::Import(std::string objFile)
{
	// Determin if file exist
	std::ifstream fin(objFile.c_str());
	if (!fin.fail()) {
		fin.close();
		ImportList.push_back(objFile);
		return true;
	}
	else {
		std::cout << "No import!" << std::endl;
		std::cin.get();
		return false;
	}
}