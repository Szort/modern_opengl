#include "Shader.h"

// Parse raw shader string from .glsl file to readable string
ShadersStringData* Shader::ShaderParser(std::string file_name)
{
	ShadersStringData* shader_strings = new ShadersStringData();
	std::ifstream file_data(std::string("GLSL/" + file_name));
	std::string line_data;
	std::stringstream parsed_string[3];
	ShaderType parsing_type = NO_SHADER;
	// [0] - vertex shader
	// [1] - geometry shader
	// [2] - fragment shader

	while(getline(file_data, line_data))
	{
		if (line_data.find("#shader") != std::string::npos)
		{
			if (line_data.find("vertex") != std::string::npos)
				parsing_type = VERTEX_SHADER;
			else if(line_data.find("geometry") != std::string::npos)
				parsing_type = GEOMETRY_SHADER;
			else if(line_data.find("fragment") != std::string::npos)
				parsing_type = FRAGMENT_SHADER;
		}
		else {
			parsed_string[(int)parsing_type] << line_data << '\n';
		}
	}

	shader_strings->vertex_string = parsed_string[VERTEX_SHADER].str();
	shader_strings->geometry_string = parsed_string[GEOMETRY_SHADER].str();
	shader_strings->fragment_string = parsed_string[FRAGMENT_SHADER].str();

	return shader_strings;
}

// Compile shader string to glProgram
void Shader::ShaderCompile(std::string file_name)
{
	std::cout << "Compiling " << file_name << " shader..." << std::endl;

	// Compile individual chanks of shader program
	ShadersStringData* shader_parsed;
	shader_parsed = ShaderParser(file_name);
	std::vector<unsigned int> shader_id;
	shader_id.resize((int)NO_SHADER - 1);

	for (int shader_type = VERTEX_SHADER; shader_type < NO_SHADER; shader_type++)
	{
		shader_id[shader_type] = 0;
		const char* shader_source;

		switch (shader_type)
		{
		case VERTEX_SHADER:
			shader_source = shader_parsed->vertex_string.c_str();
			if (*shader_source == '\0')
				continue;
			shader_id[shader_type] = glCreateShader(GL_VERTEX_SHADER);
			break;
		case GEOMETRY_SHADER:
			shader_source = shader_parsed->geometry_string.c_str();
			if (*shader_source == '\0')
				continue;
			shader_id[shader_type] = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		case FRAGMENT_SHADER:
			shader_source = shader_parsed->fragment_string.c_str();
			if (*shader_source == '\0')
				continue;
			shader_id[shader_type] = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		default:
			break;
		}
		
		glShaderSource(shader_id[shader_type], 1, &shader_source, NULL);
		glCompileShader(shader_id[shader_type]);

		int compilation_info;
		glGetShaderiv(shader_id[shader_type], GL_COMPILE_STATUS, &compilation_info);
		if (compilation_info == GL_FALSE)
		{
			int lenght;
			glGetShaderiv(shader_id[shader_type], GL_INFO_LOG_LENGTH, &lenght);
			char* message = (char*)alloca(lenght * sizeof(char));
			glGetShaderInfoLog(shader_id[shader_type], lenght, &lenght, message);
			std::cout << message << std::endl;
			std::cout << "Failed to compile ";
		}
		else
		{
			std::cout << "Compiled ";
		}

		if (shader_type == VERTEX_SHADER)
			std::cout << "vertex";
		else if (shader_type == GEOMETRY_SHADER)
			std::cout << "geometry";
		else if (shader_type == FRAGMENT_SHADER)
			std::cout << "fragment";

		std::cout << " shader! " << "ID: " << shader_id[shader_type] << std::endl;
	}

	// Assemble whole shader program
	program_id = glCreateProgram();
	for (int shader_type = 0; shader_type < NO_SHADER; shader_type++)
	{
		if (shader_id[shader_type] != 0)
			glAttachShader(program_id, shader_id[shader_type]);
	}

	// Link shader program
	glLinkProgram(program_id);

	// We should delete individual shaders
	for (int shader_type = 0; shader_type < NO_SHADER; shader_type++)
	{
		if (shader_id[shader_type] != 0)
			glDeleteShader(shader_id[shader_type]);
	}

	// Show program id
	std::cout << "Program ID: " << program_id << std::endl;
}

void Shader::BindShader()
{
	glUseProgram(program_id);
}

void Shader::UnbindShader()
{
	glUseProgram(0);
}