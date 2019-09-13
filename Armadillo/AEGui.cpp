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

void AEGui::Draw(AEViewport& viewport)
{
	//ImGui::BeginDocked(context->CurrentWindow, NULL);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	{
		float f = 0.0f;
		int counter = 0;

		//ImGui::BeginMainMenuBar();
		//ImGui::Text("Main Menu Bar.");
		//ImGui::EndMainMenuBar();

		//bool open_w = true;
		//ImDrawList* draw_list = ImGui::GetWindowDrawList();
		//const ImRect pickRc(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
		//draw_list->AddRectFilled(pickRc.Min, pickRc.Max, 0xFF000000);

		//ImGui::SetNextWindowContentSize(ImVec2(200, 200));
		//ImGui::BeginTabBar("asd", ImGuiTabBarFlags_DockNode);
		//ImGui::EndTabBar();
		//
		//
		//ImGui::SetNextWindowContentSize(ImVec2(200, 200));
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		//static bool menu = false;
		//ImGui::Begin("Test #1490, v2", NULL, ImGuiWindowFlags_AlwaysAutoResize | (menu ? ImGuiWindowFlags_MenuBar : 0));
		//ImVec2 p1 = ImGui::GetCursorScreenPos();
		//ImVec2 p2 = ImVec2(p1.x + 200, p1.y + 200);
		//ImGui::Checkbox("Menu", &menu);
		//ImGui::GetWindowDrawList()->AddLine(p1, p2, IM_COL32(255, 0, 255, 255));
		//ImGui::GetWindowDrawList()->AddCircleFilled(p1, 6.0f, IM_COL32(255, 0, 255, 255));
		//ImGui::GetWindowDrawList()->AddCircleFilled(p2, 6.0f, IM_COL32(255, 0, 255, 255));
		//ImGui::End();
		//ImGui::PopStyleVar();


		// Set up docked layout
		//ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
		//ImGui::DockBuilderAddNode(dockspace_id); // Add empty node
		//ImGui::DockBuilderSetNodeSize(dockspace_id, dockspace_size);
		////
		////dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
		////ImGuiID dock_id_prop = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, NULL, &dock_main_id);
		//ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, NULL, &dock_main_id);
		////
		//ImGui::DockBuilderDockWindow("Log", dock_id_bottom);
		//ImGui::DockBuilderFinish(dockspace_id);

		ImGui::Begin("Hello, world!");
		
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);
		
		ImGui::SliderFloat("float", &viewport.FpsCap, 1.0f, 120.0f);    // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
		
		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);
		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
			1000.0f / ImGui::GetIO().Framerate, 
			ImGui::GetIO().Framerate);
		
		ImGui::Text("Camera direction %.3f : %.3f : %.3f", 
			viewport.currentCamera->Direction.x, 
			viewport.currentCamera->Direction.y, 
			viewport.currentCamera->Direction.z);
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