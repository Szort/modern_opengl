#pragma once

#include "AECamera.h"
#include <GLM/glm.hpp>

class AEViewport
{
	GLFWwindow*		window;
	AECamera*		currentCamera;
	glm::ivec2		size;

public:
	AEViewport() {};
	AEViewport(bool& complete, uint32_t view_x, uint32_t view_y);
	~AEViewport() {};

	void CheckForExtesions(std::vector<const char*>& extensions);
	void ProcessInput();
	void Destroy();

	GLFWwindow* GetWindow() { return window; };
	AECamera*	GetCurrentCamera() { return currentCamera; };
	glm::ivec2* GetSize() { return &size; };

	void SetCurrentCamera(AECamera* cam) { currentCamera = cam; };
};