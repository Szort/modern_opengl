#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

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