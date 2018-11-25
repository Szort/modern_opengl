#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

enum ShaderType
{
	VERTEX_SHADER,
	GEOMETRY_SHADER,
	FRAGMENT_SHADER
};

struct ShadersStringData
{
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
};