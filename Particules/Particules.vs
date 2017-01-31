#version 420

const GLChar * vertex_shader_src = GLSL(
in float in_value;
out float out_value;

void main()
{
	out_value = sqrt(in_value);
}
);