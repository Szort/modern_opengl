#shader vertex
#version 450

layout(location = 1) vec4 position;

void main()
{
	gl_Position = position;
}

#shader fragment
#version 450

void main()
{
	vec4 out_color = vec4(0.8, 0.2, 0.0, 1.0);
	gl_FragColor = out_color;
}