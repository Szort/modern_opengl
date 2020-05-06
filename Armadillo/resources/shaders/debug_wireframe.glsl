#shader vertex
#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

layout(binding = 0) uniform globalBuffer
{
    mat4	CameraVPMatrix;
	mat4	CameraPMatrix_Inv;
	vec3	AmbientColor;
	float	padding0;
};

struct ObjectData
{
	vec2 x_minmax;
	vec2 y_minmax;
	vec2 z_minmax;
	vec2 padding0;
    mat4 matrix;
};

layout(std140, binding = 0) buffer objectsDataBuffer
{
    ObjectData obj[];
};

void main()
{
	// Need to construct separate DrawCommandList to draw selection
	gl_Position = CameraVPMatrix * obj[0].matrix * vec4(position, 1.0);
}

#shader fragment
#version 450

layout(location = 0) out vec3 color;

void main()
{
	color = vec3(0.0, 1.0, 0.0);
}