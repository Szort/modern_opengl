#pragma once

#include <GLM/glm.hpp>
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Render.h"
#include "Diagnostics.h"
#include "AELight.h"

class Armadillo
{
public:
	Armadillo() {};
	~Armadillo() {};

	void ViewDiagnosticGraph();
};