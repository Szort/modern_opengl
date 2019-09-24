#shader vertex
#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;
layout(location = 4) in uint drawid;

layout(std140, binding = 0) buffer matBuffer
{
    mat4 matrix[];
};

uniform mat4 ViewProjectionMatrix;

out vec4 FragPos;
out vec3 VertColor;
out vec3 NormalDir;
out vec2 TextCoord;

void main()
{
	VertColor = color;
	NormalDir = normal;
	TextCoord = texCoord;
	FragPos = matrix[drawid] * position;
	gl_Position = ViewProjectionMatrix * matrix[drawid] * position;
}

#shader fragment
#version 450

in vec4 FragPos;
in vec3 VertColor;
in vec3 NormalDir;
in vec2 TextCoord;

const vec3 ambient = vec3(0.0f, 0.1f, 0.2f);
const vec3 lightPos = vec3(20, 10, 10);
const vec3 lightColor = vec3(1.0f, 0.3f, 0.0f);

void main()
{
	vec3 norm = normalize(NormalDir);
	vec3 lightDir = normalize(lightPos - FragPos.xyz); 
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	vec3 result = ambient + diffuse;
	gl_FragColor = vec4(result, 1.0);
}