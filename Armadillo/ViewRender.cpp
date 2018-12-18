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

	// Compile shader
	shader_basic = new Shader();
	shader_basic->ShaderCompile("basic.glsl");

	// Vertex buffer object
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	return viewport_window;
}

void ViewRender::Render()
{
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(viewport_window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader_basic->BindShader();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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