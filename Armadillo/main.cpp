/*====================================
			   Armadillo
	  Test ground for crazy idea.

		Author: Krzysztof Flisik
====================================*/

#include <stdlib.h>
#include <algorithm>

#include "AECore.h"
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
	AEViewport* Viewport = new AEViewport(success, 1920, 1080);
	if (!success) return 0;

	Viewport->CheckForExtesions(extensions);

	// Global resources
	AEFrameBuffer* FrameImage = new AEFrameBuffer();
	AEEngine* Engine = new AEEngine();
	AEScene* Scene = new AEScene();
	AECamera* Camera = new AECamera();
	AEGui* GUI = new AEGui();

	AEPrimitive		Plane(eAE_PrimitiveType_Plane);

	// Startup light setup
	AELight			PointLight01(eAE_LightType_Point);
	PointLight01.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	PointLight01.SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));

	// Get current cam for viewport
	Viewport->SetCurrentCamera(Camera);

	// Import assets
	Scene->Add(Plane);
	Scene->Add(PointLight01);
	Scene->ImportAsset("./resources/meshes/Sponza/Sponza.gltf");
	//Scene.ImportAsset("./resources/meshes/BoxVertexColors.gltf");
	//Scene.ImportAsset("./resources/meshes/Lucy/lucy_20L.OBJ");

	// Create GBuffer
	FrameImage->CreateFrameBuffer(Viewport);

	// Construct buffer data from imported meshes
	Engine->CompileVAO();
	Engine->ConstructData(Scene);

	// Compile shaders
	AEShader Shader_Basic, Shader_Show, Shader_Wire, Shader_BBox;
	Shader_Basic.Compile("geometry_opaque.glsl");
	Shader_Show.Compile("light_draw.glsl");
	Shader_Wire.Compile("debug_wireframe.glsl");
	Shader_BBox.Compile("debug_bbox.glsl");

	//Initialize resources
	GUI->Initiate(Viewport->GetWindow());

	Engine->GlobalUBO.AmbientColor = glm::vec3(0.2f, 0.22f, 0.17f);

	// Compile geometry data
	Engine->CopyData_GPU();
	Scene->ConstructBuffers();
	// Bind GBuffer textures in manual fashion.
	// uint64_t are not present in GLSL shaders on Intel.
	// If we want a minimum CPU overhead we can pass all textures we have in one command,
	// but we need to pass existing resources.
	glBindTextures(0, eAE_GBuffer_Count, FrameImage->GetTexture());

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(Viewport->GetWindow()))
	{
		// Controlls input section
		//-------------------------------------------------------------------
		Viewport->ProcessInput();

		// Pre frame setup section
		//-------------------------------------------------------------------
		glEnable(GL_DEPTH_TEST); // Enable depth-testing
		glDepthFunc(GL_LESS); // Depth-testing interprets a smaller value as "closer"
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);

		//Clear buffer before rendering
		glClearColor(GUI->clear_color.x, GUI->clear_color.y, GUI->clear_color.z, GUI->clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Update data and parameters before rendering
		//-------------------------------------------------------------------
		Engine->GlobalUBO.CameraVPMatrix = Camera->GetVPMatrix();
		Engine->UpdateUBO_GPU();

		// Rendering section
		//-------------------------------------------------------------------
		// Bind framebuffer to render
		FrameImage->Bind();
		FrameImage->BindForDraw();
		Engine->BindVAO();

		// Bind geometry shader and draw geometry (GBuffer 1st pass)
		Shader_Basic.Bind();		
		Engine->DrawGeometry();

		// Draw full screen quad with GBuffer textures
		FrameImage->Unbind();
		Shader_Show.Bind();
		Engine->DrawQuad();

		// Debug section
		//-------------------------------------------------------------------
		if (Engine->DebugBBox)
		{
			// Switch off depth testing. Active depth map is from full quad drawing
			glDisable(GL_DEPTH_TEST);

			// Draw debug BBx data
			Shader_BBox.Bind();
			Engine->DrawBoundingBox();

			if (false)
			{
				// Draw wireframe selected object
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				Shader_Wire.Bind();
				Engine->DrawSelected();
			}
		}

		// Unbind resources when finished to mantain order
		//-------------------------------------------------------------------
		Engine->UnbindVAO();

		// GUI draw section
		//-------------------------------------------------------------------
		GUI->Draw(Viewport, Engine);
		Engine->Idle();

		// Swap front and back buffers section
		//-------------------------------------------------------------------
		glfwSwapBuffers(Viewport->GetWindow());
		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}