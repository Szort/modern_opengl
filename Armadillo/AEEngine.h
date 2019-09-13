#pragma once

#include <vector>
#include "AEShader.h"

class AEEngine
{
	std::vector<AEShader> Shaders;

public:
	double render_time;
	double sleep_time;

	AEEngine(): render_time(0), sleep_time(0) {};
	~AEEngine() {};
}; 