
// INPUTS du Fragment Shader = OUTPUTS du Vertex Shader

// NEW --->
varying vec2 v_TexCoords;
uniform sampler2D u_Texture;
// <------

varying vec4 v_Color;

void main(void)
{
    // NEW --->
    // Ici il est necessaire d'inverser l'axe Y des coordonnees de texture
    // car l'image possede une origine en haut a gauche (top left) tandis
    // qu'en OpenGL l'origine se trouve en bas a gauche (bottom left)
    vec2 texCoords = vec2(v_TexCoords.x, 1.0 - v_TexCoords.y);
    vec4 texColor = texture2D(u_Texture, texCoords);
    // <------

    gl_FragColor = texColor;
    // decommenter pour melanger couleur et texture
    //gl_FragColor = v_Color * texColor;
}
