//====================================
//			Armadillo
//	Test ground for crazy idea.
//
//			Enjoy ;)
//
//	Author: Krzysztof Flisik
//
//====================================

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>
#include <stdlib.h>
#include <algorithm>

#include "AEViewport.h"
#include "AELight.h"
#include "AEGui.h"
#include "AEEngine.h"

int main()
{
	AEEngine* Engine = new AEEngine();
	AEGui* GUI = new AEGui();

	AECamera* Camera = new AECamera();
	Engine->currentCamera = Camera;

	// Create viewport to render
	AEViewport* Viewport = new AEViewport();
	if (!Viewport->Init(1280, 720))
		return 0;

	GUI->Initiate(Viewport->GetWindow());

	// Prepare resources
	Viewport->PrepResources();
	unsigned int vp_location_id = glGetUniformLocation(Viewport->shader_basic->program_id, "ViewProjectionMatrix");
	unsigned int m_location_id = glGetUniformLocation(Viewport->shader_basic->program_id, "ModelMatrix");

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(Viewport->GetWindow()))
	{
		Engine->ProcessInput(Viewport->GetWindow());

		//Viewport->Render();
		glEnable(GL_DEPTH_TEST); // Enable depth-testing
		glDepthFunc(GL_LESS); // Depth-testing interprets a smaller value as "closer"

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);

		//Clear buffer before rendering
		glClearColor(GUI->clear_color.x, GUI->clear_color.y, GUI->clear_color.z, GUI->clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind resources
		Viewport->shader_basic->BindShader();
		Viewport->geometry_basic->BindGeometry();
		glUniformMatrix4fv(vp_location_id, 1, GL_FALSE, &Camera->GetVPMatrix()[0][0]);
		glUniformMatrix4fv(m_location_id, 1, GL_FALSE, &Viewport->geometry_basic->GetModelMatrix()[0][0]);

		// Draw binded geometry and shader when in use
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		GUI->Draw(Engine);

		// Sleeping time
		Engine->sleep_time = (1000 / Engine->fps_cap) - Engine->render_time;
		Engine->sleep_time = Engine->sleep_time < 0 ? 0 : Engine->sleep_time;
		Sleep(Engine->sleep_time);

		// Swap front and back buffers 
		glfwSwapBuffers(Viewport->GetWindow());
		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();

	// Clean up after your self
	GUI->Destroy();
	
	delete Engine;
	delete GUI;
	delete Camera;
	delete Viewport;

	return 0;
}