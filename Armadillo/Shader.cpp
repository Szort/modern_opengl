#include "Shader.h"

ShadersStringData* Shader::ShaderParser(std::string file_name)
{
	ShadersStringData* shader_strings = nullptr;

	std::string file_path = std::string("GLSL/" + file_name);
	std::fstream file_data;
	std::string line_data;
	std::stringstream parsed_string[3];
	ShaderType parsing_type;
	// [0] - vertex shader
	// [1] - geometry shader
	// [2] - fragment shader

	file_data.open(file_path, std::ios::in);
	if (file_data.good() == true)
	{
		std::cout << "File " << file_name.c_str() << " opened! \n";
		shader_strings = new ShadersStringData();
	}
	else
	{
		std::cout << "File " << file_name.c_str() << " don't exist! \n";
		return shader_strings;
	}

	while (!file_data.eof())
	{
		std::getline(file_data, line_data);

		if (!line_data.find("#vertexshader"))
			parsing_type = VERTEX_SHADER;
		else if (!line_data.find("#geometryshader"))
			parsing_type = GEOMETRY_SHADER;
		else if (!line_data.find("#fragmentshader"))
			parsing_type = FRAGMENT_SHADER;
 
		parsed_string[parsing_type] << std::string("\"") << line_data << std::string("\\n") << std::string("\"");
	}

	std::cout << parsed_string[0].str();

	file_data.close();

	return shader_strings;
}