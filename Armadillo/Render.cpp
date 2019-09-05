#include "Render.h"

// Prepare all resources for rendering
void ViewRender::PrepResources()
{
	// Compile shader
	shader_basic = new Shader();
	shader_basic->ShaderCompile("basic.glsl");

	// Geometry buffer VBO and VAO
	geometry_basic = new Geometry();
	geometry_basic->Init();
}

// Init renderer window
bool ViewRender::Init(int view_x, int view_y)
{
	// Initialize the library
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only

	// Create a windowed mode window and its OpenGL context
	viewport_window = glfwCreateWindow(view_x, view_y, "Armadillo Viewport", NULL, NULL);
	if (!viewport_window)
	{
		glfwTerminate();
		return false;
	}

	// Make the window's context current
	glfwMakeContextCurrent(viewport_window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
		return false;
	}

	return true;
}

void ViewRender::Render()
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
	glfwSwapBuffers(viewport_window);
	
	// Poll for and process events
	glfwPollEvents();
}

void ViewRender::Destroy()
{
	glfwTerminate();
}

GLFWwindow* ViewRender::GetViewportHandle()
{
	return viewport_window;
}