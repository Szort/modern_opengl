#include "AEViewport.h"

// Init renderer window
AEViewport::AEViewport(bool& complete, uint32_t view_x, uint32_t view_y)
{
	// Initialize the library
	if (!glfwInit())
		complete = false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(view_x, view_y, "Armadillo", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		complete = false;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error in GLEW initialization!" << std::endl;
		complete = false;
	}

	size = glm::ivec2(view_x, view_y);

	complete = true;
}

void AEViewport::CheckForExtesions(std::vector<const char*>& extensions)
{
	// Checek for supportet extensions
	for (std::vector<const char*>::iterator ext_itr = extensions.begin(); ext_itr != extensions.end(); ext_itr++)
	{
		if (glewIsSupported(*ext_itr) == GL_TRUE) {
			std::cout << "SUPPORTED   " << *ext_itr << std::endl;
		}
		else {
			std::cout << "MISSING     " << *ext_itr << std::endl;
		}
	}

	std::cout << std::endl;
}

void AEViewport::ProcessInput()
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

void AEViewport::Destroy()
{
	glfwTerminate();
}