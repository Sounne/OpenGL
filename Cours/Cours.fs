
// INPUTS du Fragment Shader = OUTPUTS du Vertex Shader

varying vec2 v_TexCoords;

uniform sampler2D u_Texture;

varying vec4 v_Color;

void main(void)
{
    vec2 texCoords = vec2(v_TexCoords.x, 1.0 - v_TexCoords.y);
    vec4 texColor = texture2D(u_Texture, texCoords);
    // variable predefinies OBLIGATOIRE en OUTPUT
    // vec4 gl_FragColor
    gl_FragColor = v_Color * texColor;
}
