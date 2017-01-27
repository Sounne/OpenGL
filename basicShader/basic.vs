
// INPUTS du Vertex Shader

// note: lorsqu'il n'y a qu'un seul 'attribute'
// on sait qu'il aura le 'location' 0

// le GPU rempli automatiquement les valeurs manquantes
// d'un attribute par 0.0 pour x,y,z et 1.0 pour w

attribute vec4 a_Position; // x,y ? ?
attribute vec4 a_Color;

varying vec4 v_Color;

uniform float u_Time;

void main(void)
{
	v_Color = a_Color;

	// variable predefinie OBLIGATOIRE en OUTPUT
	// varying vec4 gl_Position
	//gl_Position = a_Position + vec4(sin(u_Time), 0.0, 0.0, sin(u_Time)); 
	//gl_Position = vec4(a_Position.xy, 0.0, 1.0);	

	// ROTATION 2D (pivot)
	mat2 rotationMatrix = mat2(
					cos(u_Time), sin(u_Time),	// 1ere colonne
					-sin(u_Time), cos(u_Time)	// 2eme colonne
				);
//0.5 + 0.5*abs(sin(u_Time)), 0.0,
	mat2 scaleMatrix = mat2(
						100.0, 0.0,
						0.0, 100.0
				);

	mat2 worldMatrix = rotationMatrix * scaleMatrix;
	float width = 1280.0;
	float height = 720.0;
	mat2 projectionMatrix = mat2(
						2.0/width, 0.0,
						0.0, 2.0/height
					);

	vec2 newPosition = projectionMatrix  
						* worldMatrix 
						* a_Position.xy;

	gl_Position = vec4(newPosition, 0.0, 1.0);

	/*
	// ROSACE POLAIRE
	float R_Theta = 2.0 * cos(4.0 * u_Time);
	gl_Position = vec4(a_Position.x * cos(u_Time) * R_Theta
				, a_Position.y * sin(u_Time) * R_Theta
				, 0.0, 1.0);
	
	// Si et seulement si primitive = GL_POINTS
	gl_PointSize = 16.0;
	*/
}
