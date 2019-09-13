#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "AEScene.h"
#include "AEShader.h"

class AEViewport
{
	GLFWwindow* window;

public:
	float FpsCap;

	AEViewport() {};
	AEViewport(bool& complete, int view_x, int view_y);
	~AEViewport() {};

	void CheckForExtesions(std::vector<const char*>& extensions);
	void PrepResources();
	void ProcessInput();
	void Render();
	void Destroy();
	GLFWwindow* GetWindow();

	AECamera*	currentCamera;
	AEShader*	shader_basic;
	AEGeometry* geometry_basic;
};