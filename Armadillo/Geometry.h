#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

static float points[] = {
   0.0f,  0.5f,  0.0f,
   0.5f, -0.5f,  0.0f,
  -0.5f, -0.5f,  0.0f
};

class Geometry
{
public:
	Geometry() {};
	~Geometry() {};

	bool Init();
	void BindGeometry();
	void UnbindGeometry();

	unsigned int vbo;
	unsigned int vao;
};