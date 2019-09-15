#include "AEViewport.h"

// Init renderer window
AEViewport::AEViewport(bool& complete, int view_x, int view_y)
{
	FpsCap = 60.0f;

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

	complete = true;
}

void AEViewport::CheckForExtesions(std::vector<const char*>& extensions)
{
	// Checek for supportet extensions
	for (std::vector<const char*>::iterator ext_itr = extensions.begin(); ext_itr != extensions.end(); ext_itr++)
	{
		if (glewIsSupported(*ext_itr) == GL_TRUE) {
			std::cout << "Extension  SUPPORTED   " << *ext_itr << std::endl;
		}
		else {
			std::cout << "Extension  MISSING     " << *ext_itr << std::endl;
		}
	}

	std::cout << std::endl;
}

// Prepare all resources for rendering
void AEViewport::PrepResources()
{
	// Compile shader
	shader_basic = new AEShader();
	shader_basic->ShaderCompile("basic.glsl");

	// Geometry buffer VBO and VAO
	geometry_basic = new AEGeometry();
}

void AEViewport::Render()
{
	glEnable(GL_DEPTH_TEST); // Enable depth-testing
	glDepthFunc(GL_LESS); // Depth-testing interprets a smaller value as "closer"

	//Clear buffer before rendering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind resources
	shader_basic->BindShader();
	geometry_basic->BindGeometry();

	// Draw binded geometry and shader when in use
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Swap front and back buffers 
	glfwSwapBuffers(window);
	
	// Poll for and process events
	glfwPollEvents();
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

GLFWwindow* AEViewport::GetWindow()
{
	return window;
}