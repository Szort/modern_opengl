#shader vertex
#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 texCoord;
layout(location = 4) in uint drawid;

layout(std140, binding = 0) uniform matBuffer
{
    mat4 CameraVPMatrix;
};

layout(std140, binding = 0) buffer modelMatrixBuffer
{
    mat4 matrix[];
};

out vec4 FragPos;
out vec4 VertColor;
out vec3 NormalDir;
out vec2 TextCoord;

void main()
{
	VertColor = color;
	NormalDir = normal;
	TextCoord = texCoord.xy;
	FragPos = matrix[drawid] * vec4(position, 1.0);
	gl_Position = CameraVPMatrix * FragPos;
}

#shader fragment
#version 450

layout(location = 0) out vec3 color;
layout(location = 1) out vec3 normal;

in vec4 FragPos;
in vec4 VertColor;
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

	normal = norm;
	color = result;
}