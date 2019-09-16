#pragma once

#include <vector>
#include "AEScene.h"
#include "AEObject.h"
#include "AEShader.h"
#include "AEDiagnostics.h"

#define VAO_POSITION_LOCATION		0
#define	VAO_COLOR_LOCATION			1
#define VAO_NORMAL_LOCATION			2
#define VAO_TEXTURECOORD_LOCATION	3
#define VAO_DRAWID_LOCATION			4

struct AEImportDataSlice
{
	unsigned int	vertex_count;
	unsigned int	indices_count;

	float*			vertex_data;
	unsigned int*	indices_data;
	float*			vertex_data_start;
	unsigned int*	indices_data_start;
};

class AEEngine
{
	std::vector<AEShader>			Shaders;

	unsigned int VAO_Static;
	unsigned int VBO_Static;
	unsigned int IBO_Static;
	unsigned int DIBO_Static;
	unsigned int DCBO_Static;

public:
	float		FpsCap;
	double		RenderTime;
	double		SleepTime;
	AEDrawList	DrawList;

	AEEngine(): FpsCap(60.0f), RenderTime(0), SleepTime(0) {};
	~AEEngine() {};

	void ConstructData(AEScene& scene);
	void CompileVAO();
	void BindVAO();
	void UnbindVAO();
	void Idle();
}; 