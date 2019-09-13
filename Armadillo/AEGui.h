#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "AEViewport.h"

class AEGui
{
	GLFWwindow* window;

public:
	ImGuiContext* context;
	ImGuiID dockspace_id;
	ImGuiID dock_main_id;
	ImVec2 dockspace_size;

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

public:
	AEGui() {};
	~AEGui() {};

	void Initiate(GLFWwindow* window);
	void Draw(AEViewport& viewport);
	void Destroy();
};