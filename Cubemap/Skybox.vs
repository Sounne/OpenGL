in vec4 a_Position;

void main(void)
{
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(a_Position.xyz, 0.0);
}

//	out vec4 gl_Position -> Rasterizer

[Vertex Shader] //	Vertex en 4D
	|
[Perspective Divide] //	Vertex en 3D
	|
	Position3D = gl_Position3D.xyz /	Position3D.w
	Positon3DCubeMap = gl_Position.xyww -> z = w -> x,y,1.0
	|
[Primitive Assembly]	//	SI GL_TRIANGLES, on attends 3 vertex
	|
	Position2D = Position3D.xy / Position3D.z
	|
[Rasterizer]