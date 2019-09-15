#pragma once

#include <vector>
#include "AEObject.h"
#include "AEShader.h"

#define VAO_POSITION_LOCATION		0
#define	VAO_COLOR_LOCATION			1
#define VAO_NORMAL_LOCATION			2
#define VAO_TEXTURECOORD_LOCATION	3
#define VAO_DRAWID_LOCATION			4

class AEEngine
{
	std::vector<AEShader> Shaders;

	unsigned int VAO_Static;
	unsigned int VBO_Static;
	unsigned int IBO_Static;
	unsigned int DIBO_Static;
	unsigned int DCBO_Static;

public:
	double		RenderTime;
	double		SleepTime;
	float		FpsCap;
	AEDrawList	DrawList;

	AEEngine(): RenderTime(0), SleepTime(0) {
		DrawList.vertex_count = 0;
		DrawList.indices_count = 0;
		DrawList.vertex_data = new float();
		DrawList.indices_data = new unsigned int();
	};
	~AEEngine() {};

	void CompileVAO();
	void BindVAO();
	void UnbindVAO();
	void Idle();
}; 