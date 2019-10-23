#shader vertex
#version 450

layout(location = 0) in vec3 position;

out int DrawID;

void main()
{
	DrawID = gl_InstanceID;
	gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}


#shader geometry
#version 450

layout(binding = 0) uniform globalBuffer
{
    mat4 CameraVPMatrix;
};

in int DrawID[];

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

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 14) out;

void main() {
	int id = DrawID[0]; 
	mat4 mvp_matrix = CameraVPMatrix * obj[id].matrix;
	vec2 x = obj[id].x_minmax;	// x - min, y - max
	vec2 y = obj[id].y_minmax;	// x - min, y - max
	vec2 z = obj[id].z_minmax;	// x - min, y - max

    gl_Position = mvp_matrix * (gl_in[0].gl_Position + vec4(x.x, y.y, z.x, 0.0)); EmitVertex();
	gl_Position = mvp_matrix * (gl_in[0].gl_Position + vec4(x.y, y.y, z.x, 0.0)); EmitVertex();
	gl_Position = mvp_matrix * (gl_in[0].gl_Position + vec4(x.x, y.x, z.x, 0.0)); EmitVertex();
	gl_Position = mvp_matrix * (gl_in[0].gl_Position + vec4(x.y, y.x, z.x, 0.0)); EmitVertex();

	gl_Position = mvp_matrix * (gl_in[0].gl_Position + vec4(x.y, y.x, z.y, 0.0)); EmitVertex();
	gl_Position = mvp_matrix * (gl_in[0].gl_Position + vec4(x.y, y.y, z.x, 0.0)); EmitVertex();
	gl_Position = mvp_matrix * (gl_in[0].gl_Position + vec4(x.y, y.y, z.y, 0.0)); EmitVertex();
	gl_Position = mvp_matrix * (gl_in[0].gl_Position + vec4(x.x, y.y, z.x, 0.0)); EmitVertex();
	
	gl_Position = mvp_matrix * (gl_in[0].gl_Position + vec4(x.x, y.y, z.y, 0.0)); EmitVertex();
	gl_Position = mvp_matrix * (gl_in[0].gl_Position + vec4(x.x, y.x, z.x, 0.0)); EmitVertex();
	gl_Position = mvp_matrix * (gl_in[0].gl_Position + vec4(x.x, y.x, z.y, 0.0)); EmitVertex();
	gl_Position = mvp_matrix * (gl_in[0].gl_Position + vec4(x.y, y.x, z.y, 0.0)); EmitVertex();
	
	gl_Position = mvp_matrix * (gl_in[0].gl_Position + vec4(x.x, y.y, z.y, 0.0)); EmitVertex();
	gl_Position = mvp_matrix * (gl_in[0].gl_Position + vec4(x.y, y.y, z.y, 0.0)); EmitVertex();
    
    EndPrimitive();
}


#shader fragment
#version 450

void main()
{
	gl_FragColor = vec4(0.4, 0.4, 1.0, 1.0);
}