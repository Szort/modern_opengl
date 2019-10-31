#shader vertex
#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 texCoord;
layout(location = 4) in uint drawid;

layout(binding = 0) uniform globalBuffer
{
    mat4	CameraVPMatrix;
	mat4	CameraPMatrix_Inv;
	vec3	AmbientColor;
	float	padding0;
};

struct ObjectData
{
	vec2	x_minmax;
	vec2	y_minmax;
	vec2	z_minmax;
	vec2	padding0;
    mat4	matrix;
};

layout(std140, binding = 0) buffer objectsBuffer
{
    ObjectData obj[];
};

struct MaterialData
{
	vec4	albedoValue;
	vec4	normalValue;
	vec4	emissiveValue;
	float	roughnessValue;
	float	metalnessValue;
	float	transparencyValue;
};

layout(std140, binding = 2) buffer materialBuffer
{
    MaterialData mat[];
};

out vec4 FragPos;
out vec4 VertColor;
out vec3 NormalDir;
out vec2 TextCoord;

void main()
{
	VertColor = color;
	TextCoord = texCoord.xy;
    NormalDir = mat3(obj[drawid].matrix) * normal;

	FragPos = obj[drawid].matrix * vec4(position, 1.0);
	gl_Position = CameraVPMatrix * FragPos;
}

#shader fragment
#version 450

layout(location = 0) out vec3 world_position;
layout(location = 1) out vec3 color;
layout(location = 2) out vec3 normal;

in vec4 FragPos;
in vec4 VertColor;
in vec3 NormalDir;
in vec2 TextCoord;

void main()
{
	world_position = FragPos.xyz;
	normal = NormalDir;
	color = vec3(0.5);
}