#pragma once

#include "AECore.h"

enum ShaderType
{
	VERTEX_SHADER,
	GEOMETRY_SHADER,
	FRAGMENT_SHADER,
	NO_SHADER
};

struct ShadersStringData
{
	ShadersStringData()
	{
		vertex_string = "";
		geometry_string = "";
		fragment_string = "";
	}

	std::string vertex_string;
	std::string geometry_string;
	std::string fragment_string;
};

class AEShader
{
	uint32_t program_id;

public:
	AEShader() {};
	~AEShader() {};

	ShadersStringData* ShaderParser(std::string file_name);
	void Compile(std::string file_name);
	void Bind();
	void Unbind();

	const uint32_t GetProgramID() { return program_id; };
};