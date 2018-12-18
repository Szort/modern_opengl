#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "Shader.h"

static float points[] = {
   0.0f,  0.5f,  0.0f,
   0.5f, -0.5f,  0.0f,
  -0.5f, -0.5f,  0.0f
};

class ViewRender
{

public:
	ViewRender() {};
	~ViewRender() {};

	GLFWwindow* Init(int view_x, int view_y);
	void Render();
	void Destroy();

protected:
	Shader* shader_basic;
	GLFWwindow* viewport_window;
	unsigned int vbo;
	unsigned int vao;
};