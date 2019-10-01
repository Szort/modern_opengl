#shader vertex
#version 450

layout(location = 0) in vec3 position;

layout(std140, binding = 0) uniform matBuffer
{
    mat4 CameraVPMatrix;
};

void main()
{
	gl_Position = CameraVPMatrix * vec4(position, 1.0);
}

#shader fragment
#version 450

layout(binding = 0) uniform sampler2D albedo;
layout(binding = 1) uniform sampler2D normal;

layout(location = 0) out vec3 fragColor;

void main()
{
	fragColor = vec3(1.0, 0.2, 0.0);
}