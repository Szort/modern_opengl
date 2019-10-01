#pragma once

#include "AECamera.h"

class AEViewport
{
	GLFWwindow* window;

public:
	AEViewport() {};
	AEViewport(bool& complete, int view_x, int view_y);
	~AEViewport() {};

	void CheckForExtesions(std::vector<const char*>& extensions);
	void ProcessInput();
	void Destroy();
	GLFWwindow* GetWindow();

	AECamera*	currentCamera;
};