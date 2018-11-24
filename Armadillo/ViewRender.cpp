#include "ViewRender.h"

GLFWwindow* ViewRender::Init(int view_x, int view_y)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return nullptr;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(view_x, view_y, "Armadillo Viewport", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return nullptr;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	return window;
}

void ViewRender::Render()
{

}

void ViewRender::Destroy()
{
	glfwTerminate();
}