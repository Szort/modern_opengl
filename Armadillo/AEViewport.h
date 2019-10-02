#pragma once

#include "AECamera.h"
#include <GLM/glm.hpp>

class AEViewport
{
	GLFWwindow*		window;
	glm::ivec2		size;

public:
	AEViewport() {};
	AEViewport(bool& complete, uint32_t view_x, uint32_t view_y);
	~AEViewport() {};

	void CheckForExtesions(std::vector<const char*>& extensions);
	void ProcessInput();
	void Destroy();

	GLFWwindow* GetWindow() { return window; };
	glm::ivec2* GetSize() { return &size; };

	AECamera*	currentCamera;
};