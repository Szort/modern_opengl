
#include "Armadillo.h"
#include <iostream>


int main()
{
	ViewRender* viewport_main = new ViewRender();
	GLFWwindow* viewport_main_window = viewport_main->Init(640, 480);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(viewport_main_window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(viewport_main_window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	viewport_main->Destroy();
	return 0;
}