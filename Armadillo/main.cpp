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
	"GL_ARB_sync",
	"GL_ARB_gpu_shader_int64",
	"GL_ARB_sparse_texture",
	"GL_ARB_multi_bind"
};

int main()
{
	// Create viewport to render
	bool success;
	AEViewport Viewport(success, 1920, 1080);
	if (!success) return 0;

	Viewport.CheckForExtesions(extensions);

	// Global resources
	AEFrameBuffer	FrameImage;
	AEEngine		Engine;
	AEScene			Scene;
	AECamera		Camera;
	AEGui			GUI;

	AEPrimitive		Plane(eAE_PrimitiveType_Plane);

	// Get current cam for viewport
	Viewport.SetCurrentCamera(&Camera);

	// Import assets
	Scene.Add(Plane);
	Scene.ImportAsset("./resources/meshes/Sponza/Sponza.gltf");
	Scene.ImportAsset("./resources/meshes/BoxVertexColors.gltf");

	// Create GBuffer
	FrameImage.CreateFrameBuffer(Viewport);

	// Construct buffer data from imported meshes
	Engine.CompileVAO();
	Engine.ConstructData(Scene);

	// Compile shaders
	AEShader Shader_Basic, Shader_Show, Shader_Wire, Shader_BBox;
	Shader_Basic.Compile("basic.glsl");
	Shader_Show.Compile("show.glsl");
	Shader_Wire.Compile("wireframe.glsl");
	Shader_BBox.Compile("show_bbox.glsl");

	//Initialize resources
	GUI.Initiate(Viewport.GetWindow());

	// Compile geometry data
	Engine.CopyData_GPU();

	// Bind GBuffer textures in manual fashion.
	// uint64_t are not present in GLSL shaders on Intel.
	// If we want a minimum CPU overhead we can pass all textures we have in one command,
	// but we need to pass existing resources.
	glBindTextures(0, eAE_GBuffer_Count, FrameImage.GetTexture());

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(Viewport.GetWindow()))
	{
		// Controlls input section
		//-------------------------------------------------------------------
		Viewport.ProcessInput();

		// Pre frame setup section
		//-------------------------------------------------------------------
		glEnable(GL_DEPTH_TEST); // Enable depth-testing
		glDepthFunc(GL_LESS); // Depth-testing interprets a smaller value as "closer"
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);

		//Clear buffer before rendering
		glClearColor(GUI.clear_color.x, GUI.clear_color.y, GUI.clear_color.z, GUI.clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Update data and parameters before rendering
		//-------------------------------------------------------------------
		Engine.GlobalUBO.CameraVPMatrix = Camera.GetVPMatrix();
		Engine.UpdateUBO_GPU();

		// Rendering section
		//-------------------------------------------------------------------
		// Bind framebuffer to render
		FrameImage.Bind();
		FrameImage.BindForDraw();
		Engine.BindVAO();

		// Bind geometry shader and draw geometry (GBuffer 1st pass)
		Shader_Basic.Bind();		
		Engine.DrawGeometry();

		// Draw full screen quad with GBuffer textures
		FrameImage.Unbind();
		Shader_Show.Bind();
		Engine.DrawQuad();

		// Debug section
		//-------------------------------------------------------------------
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_DEPTH_TEST);

		// Draw wireframe selected object
		Shader_Wire.Bind();
		Engine.DrawSelected();

		// Draw debug BBx data
		if (Engine.DebugBBox) {
			Shader_BBox.Bind();
			Engine.DrawBoundingBox();
		}

		// Unbind resources when finished to mantain order
		//-------------------------------------------------------------------
		Engine.UnbindVAO();

		// GUI draw section
		//-------------------------------------------------------------------
		GUI.Draw(Viewport, Engine);
		Engine.Idle();

		// Swap front and back buffers section
		//-------------------------------------------------------------------
		glfwSwapBuffers(Viewport.GetWindow());
		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}