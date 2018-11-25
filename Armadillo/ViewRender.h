#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "Shader.h"

class ViewRender
{

public:
	ViewRender() {};
	~ViewRender() {};

	GLFWwindow* Init(int view_x, int view_y);
	void Render();
	void Destroy();

	Shader* shader_basic;
};