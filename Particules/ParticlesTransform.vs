#version 420

layout(location = 0) in vec3 a_Position
layout(location = 1) in vec3 a_Velocity;

uniform float u_Deltatime;

out vec3 o_Position;
out vec4 o_Velocity;

void main(void)
{
	const vec3 acceleration = vec3(0.0, -9.8, 0.0);
	const vec3 acceleration2 = acceleration * u_Deltatime * 0.5;

	o_Velocity = a_Position + o_Velocity * u_Deltatime + acceleration2;
	o_Position = a_Velocity + acceleration;
}