#include "Armadillo.h"
#include <iostream>
#include <stdlib.h>
#include <algorithm>

int main()
{
	// Timer for whole rendering. Need for diagnostics
	DiagTimer generalTimer;
	double fps_cap = 60;
	double render_time = 0;
	double elapsed_time = 0;
	double sleep_time = 0;

	// Create viewport to render
	ViewRender* viewport_main = new ViewRender();
	if (!viewport_main->Init(1280, 720))
		return 0;

	// Prepare resources
	viewport_main->PrepResources();

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(viewport_main->GetViewportHandle()))
	{	
		elapsed_time = 0;
		generalTimer.StartTimer();

		// Render scene
		viewport_main->Render();

		// Get time in milisec how long rendering take
		render_time = generalTimer.GetTimer();
		std::cout << "Render only time: " << render_time << " ms" << std::endl;

		// Sleeping time
		sleep_time = (1000 / fps_cap) - render_time;
		sleep_time = sleep_time < 0 ? 0 : sleep_time;
		std::cout << "Sleep time: " << sleep_time << " ms" << std::endl;
		Sleep(sleep_time);

		// How long it take 
		elapsed_time = generalTimer.GetTimer();
		std::cout << "Frame time caped: " << elapsed_time << " ms" << std::endl;
		std::cout << std::endl;
	}

	// Destroy viewport if we close it
	viewport_main->Destroy();

	return 0;
}