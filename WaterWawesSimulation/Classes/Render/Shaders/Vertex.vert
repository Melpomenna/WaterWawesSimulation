#version 440 core

layout(location=0) in vec4 pos;

layout(location=1) in vec4 color;
layout(location=2) out vec4 frag_color;

layout(location=3) in vec3 t_coords;
layout(location=4) out vec3 out_t_coords;

layout(location=20) uniform mat4 u_model;
layout(location=21) uniform mat4 u_view;
layout(location=22) uniform mat4 u_projection;

void main()
{
	gl_Position = pos*u_model*u_view*u_projection;
	frag_color = color;
	out_t_coords = t_coords;
}