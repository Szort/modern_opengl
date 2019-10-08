#shader vertex
#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 texCoord;
layout(location = 4) in uint drawid;

out vec2 TexCoord;

void main()
{
	TexCoord = texCoord.xy;
	gl_Position = vec4(position, 1.0);
}

#shader fragment
#version 450

in vec2 TexCoord;

layout(binding = 0) uniform sampler2D albedo;
layout(binding = 1) uniform sampler2D normal;
layout(binding = 2) uniform sampler2D depth;

layout(binding = 0) uniform globalBuffer
{
    mat4 CameraVPMatrix;
};

layout(location = 0) out vec3 fragColor;

void main()
{
	vec3 outColor = texture(albedo, TexCoord).xyz;
	fragColor = outColor;
}