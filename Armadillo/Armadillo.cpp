#include "Armadillo.h"
#include <iostream>
#include <stdlib.h>
#include <algorithm>

int main()
{
	// Timer for whole rendering. Need for diagnostics
	DiagTimer renderTimer;
	DiagTimer allTimer;
	double fps_cap = 80;
	double render_time = 0;
	double sleep_time = 0;
	unsigned frame_counter = 0;

	// Create viewport to render
	ViewRender* viewport_main = new ViewRender();
	if (!viewport_main->Init(1280, 720))
		return 0;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	const char* glsl_version = "#version 150";
	ImGui_ImplGlfw_InitForOpenGL(viewport_main->GetViewportHandle(), true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Prepare resources
	viewport_main->PrepResources();

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(viewport_main->GetViewportHandle()))
	{	
		//viewport_main->Render();
		glEnable(GL_DEPTH_TEST); // Enable depth-testing
		glDepthFunc(GL_LESS); // Depth-testing interprets a smaller value as "closer"

		//Clear buffer before rendering
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind resources
		viewport_main->shader_basic->BindShader();
		viewport_main->geometry_basic->BindGeometry();

		// Draw binded geometry and shader when in use
		glDrawArrays(GL_TRIANGLES, 0, 3);



		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



		// Swap front and back buffers 
		glfwSwapBuffers(viewport_main->GetViewportHandle());

		// Poll for and process events
		glfwPollEvents();


		

		// Count frame
		//std::cout << "Frame #: " << frame_counter << std::endl;
		//frame_counter++;
		//
		//// Start diagnostic timers
		//renderTimer.StartTimer();
		//allTimer.StartTimer();
		//
		//// Get time in milisec how long rendering take
		//render_time = renderTimer.GetTimer();
		//std::cout << "Render only time: " << render_time << " ms" << std::endl;
		//
		// Sleeping time
		sleep_time = (1000 / fps_cap) - render_time;
		sleep_time = sleep_time < 0 ? 0 : sleep_time;
		std::cout << "Sleep time: " << sleep_time << " ms" << std::endl;
		Sleep(sleep_time);
		//
		//// How long it take 
		//std::cout << "Frame time caped: " << allTimer.GetTimer() << " ms" << std::endl;
		//std::cout << std::endl;
	}

	// Destroy viewport if we close it
	viewport_main->Destroy();

	return 0;
}

void Armadillo::ViewDiagnosticGraph()
{
	
}