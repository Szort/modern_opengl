#pragma once

#include <windows.h>
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "AEGeometry.h"

class AEViewport
{
	GLFWwindow* window;

public:
	AEViewport() {};
	~AEViewport() {};

	bool Init(int view_x, int view_y);
	void PrepResources();
	void Render();
	void Destroy();
	GLFWwindow* GetWindow();

	Shader* shader_basic;
	AEGeometry* geometry_basic;
};