#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>

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
public:
	AEShader() {};
	~AEShader() {};

	ShadersStringData* ShaderParser(std::string file_name);
	void ShaderCompile(std::string file_name);
	void Bind();
	void Unbind();

	unsigned int program_id;
};