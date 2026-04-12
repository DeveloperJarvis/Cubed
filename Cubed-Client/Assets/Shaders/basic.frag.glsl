#version 460 core

precision mediump float;

layout(location = 0) in vec3 in_color;

layout(location = 0) out vec4 out_color;

layout(push_constant) uniform PushConstants
{
	vec4 Color;
} u_PushConstants;

void main()
{
	out_color = u_PushConstants.Color;
}
