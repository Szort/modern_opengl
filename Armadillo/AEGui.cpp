#include "AEGui.h"

void AEGui::Initiate(GLFWwindow* window)
{
	this->window = window;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	context = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	dockspace_id = context->ActiveId;
	dockspace_size = ImVec2(150.0f, 100.0f);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");
}

void AEGui::Draw(AEViewport& viewport, AEEngine& engine)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	{
		ImGui::Begin("Hello, world!");
		
		ImGui::SliderFloat("float", &engine.FpsCap, 1.0f, 120.0f);    // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
			1000.0f / ImGui::GetIO().Framerate, 
			ImGui::GetIO().Framerate);
		
		ImGui::Text("PickedID: %d", engine.GetPickedID());
		ImGui::InputInt("Base:", &engine.SelectedID);
		ImGui::Checkbox("Debug Bounding Boxes", &engine.DebugBBox);

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void AEGui::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}