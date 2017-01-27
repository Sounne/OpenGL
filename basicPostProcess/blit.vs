
// INPUTS du Vertex Shader

// note: lorsqu'il n'y a qu'un seul 'attribute'
// on sait qu'il aura le 'location' 0

// le GPU rempli automatiquement les valeurs manquantes
// d'un attribute par 0.0 pour x,y,z et 1.0 pour w

attribute vec4 a_Position; // x,y,z,?

varying vec2 v_TexCoords;


void main(void)
{
// converti des coordonnees exprimees entre [-1;+1] vers [0;1]
	v_TexCoords = a_Position.xy * 0.5 + 0.5;
	gl_Position = a_Position;
}
