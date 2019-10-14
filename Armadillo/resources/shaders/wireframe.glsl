#shader vertex
#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 texCoord;
layout(location = 4) in uint drawid;

layout(binding = 0) uniform globalBuffer
{
    mat4 CameraVPMatrix;
};

struct ObjectData
{
	vec4 id;
    mat4 matrix;
};

layout(std140, binding = 0) buffer modelMatrixBuffer
{
    ObjectData obj[];
};

void main()
{
	gl_Position = CameraVPMatrix * obj[drawid].matrix * vec4(position, 1.0);
}

#shader fragment
#version 450

layout(location = 0) out vec3 color;

void main()
{
	color = vec3(0.0, 1.0, 0.0);
}