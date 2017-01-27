
// INPUTS du Vertex Shader

attribute vec4 a_Position; 

// NEW --->
attribute vec2 a_TexCoords;
varying vec2 v_TexCoords;
// <------

varying vec4 v_Color;

uniform float u_Time;

void main(void)
{
	// NEW --->
	v_TexCoords = a_TexCoords;
	// <------

	v_Color = vec4(abs(a_Position.xyz), 1.0);

	// variable predefinie OBLIGATOIRE en OUTPUT
	// varying vec4 gl_Position

	// ROTATION 3D (pivot Y)
	mat4 rotationMatrix = mat4(
					cos(u_Time), 0.0, -sin(u_Time), 0.0, 	// 1ere colonne					
					0.0, 1.0, 0.0, 0.0,						// 2eme colonne
					sin(u_Time), 0.0, cos(u_Time), 0.0,		// 3eme colonne
					0.0, 0.0, 0.0, 1.0						// 4eme colonne
				);

	mat4 scaleMatrix = mat4(
						0.10, 0.0, 0.0, 0.0,
						0.0, 0.10, 0.0, 0.0,
						0.0, 0.0, 0.10, 0.0,
						0.0, 0.0, 0.0, 1.0
				);
				
	mat4 translationMatrix = mat4(
						1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						0.0, 0.0, -2.0, 1.0
				);				

	mat4 worldMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	
	// Projection perspective
	float width = 1280.0; float height = 720.0;
	float aspectRatio = width / height;		// pour que les pixels soient "carres"
	float fovY = radians(45.0);				// degree d'ouverture = field of view
	float near = 0.1;						// distance minimale avant laquelle on "clip" le rendu
	float far = 1000.0;						// distance maximale apres laquelle on "clip" le rendu
	
	float d = 1.0 / tan(fovY/2.0);			// distance focale (oeil->plan de projection)
	float range = 1.0 / (near - far);		// normalisation en NDC de la profondeur
	mat4 projectionMatrix = mat4(
						d / aspectRatio, 0.0, 0.0, 0.0,
						0.0, d, 0.0, 0.0,
						0.0, 0.0, (near+far)*range, -1.0,
						0.0, 0.0, (near*far*2.0)*range, 0.0
					);



	gl_Position = projectionMatrix * worldMatrix * a_Position;
}
