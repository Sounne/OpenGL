#version 420

layout (location = 0) in vec4 a_Position;
layout (location = 1) in vec4 a_Color;

out4 v_Color;

layout (std140, binding = 0) uniform Matrices
{
	mat4 u_ViewMatrix;
	mat4 u_ProjectionMatrix;
};

void main(void)
{
	v_Color = a_Color;
	gl_Position = u_ProjectionMatrix *u_ViewMatrix;
}