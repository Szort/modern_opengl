#include "ViewRender.h"

GLFWwindow* ViewRender::Init(int view_x, int view_y)
{
	/* Initialize the library */
	if (!glfwInit())
		return nullptr;

	/* Create a windowed mode window and its OpenGL context */
	viewport_window = glfwCreateWindow(view_x, view_y, "Armadillo Viewport", NULL, NULL);
	if (!viewport_window)
	{
		glfwTerminate();
		return nullptr;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(viewport_window);

	/* Initialize GLEW */
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	shader_basic = new Shader();
	shader_basic->ShaderCompile("basic.glsl");

	return viewport_window;
}

void ViewRender::Render()
{
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(viewport_window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
	
		/* Swap front and back buffers */
		glfwSwapBuffers(viewport_window);
	
		/* Poll for and process events */
		glfwPollEvents();
	}

	Destroy();
}

void ViewRender::Destroy()
{
	glfwTerminate();
}