#pragma once

#include <windows.h>
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Geometry.h"

class ViewRender
{

public:
	ViewRender() {};
	~ViewRender() {};

	bool Init(int view_x, int view_y);
	void PrepResources();
	void Render();
	void Destroy();

protected:
	GLFWwindow* viewport_window;
	Shader* shader_basic;
	Geometry* geometry_basic;
};