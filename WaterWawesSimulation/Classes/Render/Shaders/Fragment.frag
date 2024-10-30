#version 440 core

layout(location=0) out vec4 out_color;
layout(location=2) in vec4 frag_color;

layout(location=4) in vec3 out_t_coords;

layout(location=10) uniform sampler3D u_sampler;

void main()
{
	vec4 sampled = texture(u_sampler, out_t_coords);
	out_color = frag_color*sampled;
}