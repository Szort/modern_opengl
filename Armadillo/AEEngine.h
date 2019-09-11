#pragma once

#include "AECamera.h"

class AEEngine
{
public:
	float fps_cap;
	double render_time;
	double sleep_time;

	AECamera* currentCamera;

	AEEngine() :
		fps_cap(60.0f)
	{};
	~AEEngine() {};

	void ProcessInput(GLFWwindow* window)
	{
		glfwGetCursorPos(window, &currentCamera->MouseCurrent_X, &currentCamera->MouseCurrent_Y);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
		{
			currentCamera->ProcessKeyboard(window);
			currentCamera->ProcessMouse(window);

			currentCamera->ComputeViewMatrix();
		}
		else {
			currentCamera->StoreMousePosition();
		}
	}
};