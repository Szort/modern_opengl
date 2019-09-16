//////////////////////////////////////
//									//
//			   Armadillo			//
//	  Test ground for crazy idea.	//
//									//
//				Enjoy ;)			//
//									//
//		Author: Krzysztof Flisik	//
//									//
//////////////////////////////////////

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>
#include <stdlib.h>
#include <algorithm>

#include "AEViewport.h"
#include "AEGui.h"
#include "AEEngine.h"
#include "AEScene.h"

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
	AEEngine	Engine;
	AEScene		Scene;
	AECamera	Camera;
	AEGui		GUI;

	Scene.Add(Camera);

	// Import assets
	Scene.Import("./resources/meshes/Sponza/Sponza.gltf");
	Scene.Import("./resources/meshes/BoxVertexColors.gltf");

	// Construct buffer data from imported meshes
	Engine.ConstructData(Scene);

	// Compile shaders
	AEShader Shader;
	Shader.ShaderCompile("basic.glsl");

	//Initialize resources
	GUI.Initiate(Viewport.GetWindow());
	Viewport.currentCamera = &Camera;

	unsigned int vp_location_id = glGetUniformLocation(Shader.program_id, "ViewProjectionMatrix");

	unsigned int ModelMatrixSSBO;
	glGenBuffers(1, &ModelMatrixSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ModelMatrixSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, Engine.DrawList.MatrixList.size() * sizeof(glm::mat4), Engine.DrawList.MatrixList.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ModelMatrixSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glObjectLabel(GL_BUFFER, ModelMatrixSSBO, -1, "ModelMatrixSSBO");	

	// Compile geometry data
	Engine.CompileVAO();

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

		// Bind resources
		Shader.BindShader();
		Engine.BindVAO();
		glUniformMatrix4fv(vp_location_id, 1, GL_FALSE, &Camera.GetVPMatrix()[0][0]);

		// Draw binded geometry and shader when in use
		glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (void*)0, (unsigned int)Engine.DrawList.CommandList.size(), 0);

		glBindVertexArray(0);

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