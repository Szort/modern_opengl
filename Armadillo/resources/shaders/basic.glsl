#shader vertex
#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;
layout(location = 2) in uint drawid;

uniform mat4 ViewProjectionMatrix;
uniform mat4 ModelMatrix;

out vec3 VertColor;

void main()
{
	VertColor = color;
	gl_Position = ViewProjectionMatrix * ModelMatrix * position;
}

#shader fragment
#version 450

in vec3 VertColor;

void main()
{
	gl_FragColor = vec4(VertColor, 1.0);
}