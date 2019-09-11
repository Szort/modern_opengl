#include "AEViewport.h"

// Prepare all resources for rendering
void AEViewport::PrepResources()
{
	// Compile shader
	shader_basic = new Shader();
	shader_basic->ShaderCompile("basic.glsl");

	// Geometry buffer VBO and VAO
	geometry_basic = new AEGeometry();
}

// Init renderer window
bool AEViewport::Init(int view_x, int view_y)
{
	// Initialize the library
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(view_x, view_y, "Armadillo Viewport", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return false;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
		return false;
	}

	return true;
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

void AEViewport::Destroy()
{
	glfwTerminate();
}

GLFWwindow* AEViewport::GetWindow()
{
	return window;
}