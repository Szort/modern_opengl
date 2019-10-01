/*====================================
			   Armadillo
	  Test ground for crazy idea.

		Author: Krzysztof Flisik
====================================*/

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>
#include <stdlib.h>
#include <algorithm>

#include "AEShader.h"
#include "AEViewport.h"
#include "AEGui.h"
#include "AEEngine.h"
#include "AEScene.h"
#include "AEFrameBuffer.h"


std::vector<const char*> extensions = {
	"GL_ARB_bindless_texture",
	"GL_ARB_buffer_storage",
	"GL_ARB_direct_state_access",
	"GL_ARB_indirect_parameters",
	"GL_ARB_map_buffer_range",
	"GL_ARB_shader_draw_parameters",
	"GL_ARB_sync"
};

int main()
{
	// Create viewport to render
	bool success;
	AEViewport Viewport(success, 1280, 720);
	if (!success) return 0;

	Viewport.CheckForExtesions(extensions);

	// Global resources
	AEFrameBuffer	FrameImage;
	AEEngine		Engine;
	AEScene			Scene;
	AECamera		Camera;
	AEGui			GUI;

	FrameImage.CreateFrameBuffer();

	// Add camera to scene
	Scene.Add(Camera);

	// Import assets
	Scene.Import("./resources/meshes/Sponza/Sponza.gltf");
	Scene.Import("./resources/meshes/BoxVertexColors.gltf");

	//Engine.RenderBuffer.CreateRenderBuffer();
	Engine.CompileVAO();

	// Construct buffer data from imported meshes
	Engine.ConstructData(Scene);

	// Compile shaders
	AEShader Shader_Basic, Shader_Show;
	Shader_Basic.ShaderCompile("basic.glsl");
	Shader_Show.ShaderCompile("show.glsl");

	//Initialize resources
	GUI.Initiate(Viewport.GetWindow());
	Viewport.currentCamera = &Camera;

	// Compile geometry data
	Engine.CopyData_GPU();

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(Viewport.GetWindow()))
	{
		Viewport.ProcessInput();

		glEnable(GL_DEPTH_TEST); // Enable depth-testing
		glDepthFunc(GL_LESS); // Depth-testing interprets a smaller value as "closer"

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);

		//Clear buffer before rendering
		glClearColor(GUI.clear_color.x, GUI.clear_color.y, GUI.clear_color.z, GUI.clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update data before rendering
		Engine.GlobalUBO.CameraVPMatrix = Camera.GetVPMatrix();
		Engine.UpdateUBO_GPU();

		// Bind framebuffer to render
		FrameImage.BindForDraw();

		// Bind resources
		Shader_Basic.Bind();
		Engine.BindVAO();

		// Draw binded geometry and shader when in use
		Engine.DrawGeometry();

		// Unbind resources when finished to mantain order
		Engine.UnbindVAO();
		FrameImage.Unbind();

		GUI.Draw(Viewport, Engine);
		Engine.Idle();

		// Swap front and back buffers 
		glfwSwapBuffers(Viewport.GetWindow());
		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}