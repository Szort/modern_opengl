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

layout(binding = 0) uniform sampler2D world_position;
layout(binding = 1) uniform sampler2D albedo;
layout(binding = 2) uniform sampler2D normal;
layout(binding = 3) uniform sampler2D depth;

layout(binding = 0) uniform globalBuffer
{
    mat4	CameraVPMatrix;
	mat4	CameraPMatrix_Inv;
	vec3	AmbientColor;
	float	padding0;
};

struct LightData
{
	vec3	position;
	float	radius;
    vec3	color;
	float	intensity;
	vec3	direction;
};

layout(std140, binding = 1) buffer lightBuffer
{
    LightData light[];
};

layout(location = 0) out vec3 fragColor;

void main()
{	
	vec3 WorldPosition = texture(world_position, TexCoord).xyz;
	vec3 Normal = normalize(texture(normal, TexCoord).xyz);

	vec3 lightDir = normalize(light[0].position - WorldPosition); 
    vec3 diffuseLight = (AmbientColor + max(dot(Normal, lightDir), 0.0)) * light[0].color;
	vec3 outColor = diffuseLight * texture(albedo, TexCoord).xyz;

	fragColor = outColor;
}