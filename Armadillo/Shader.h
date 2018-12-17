#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>

enum ShaderType
{
	NO_SHADER = -1,
	VERTEX_SHADER = 0,
	GEOMETRY_SHADER,
	FRAGMENT_SHADER
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

class Shader
{
public:
	Shader() {};
	~Shader() {};

	ShadersStringData* ShaderParser(std::string file_name);
	void ShaderCompile(std::string file_name);

	unsigned int vertex_id;
	unsigned int geometry_id;
	unsigned int fragment_id;
};