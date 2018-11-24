#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

class ViewRender
{

public:
	ViewRender() {};
	~ViewRender() {};

	GLFWwindow* Init(int view_x, int view_y);
	void Render();
	void Destroy();
};