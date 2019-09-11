#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "AEEngine.h"

class AEGui
{
	GLFWwindow* window;

public:
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

public:
	AEGui() {};
	~AEGui() {};

	void Initiate(GLFWwindow* window);
	void Draw(AEEngine* engine);
	void Destroy();
};