
// INPUTS du Fragment Shader = OUTPUTS du Vertex Shader

varying vec4 v_Color;

void main(void)
{
    // variable predefinies OBLIGATOIRE en OUTPUT
    // vec4 gl_FragColor

    gl_FragColor = v_Color;
    //gl_FragColor = glvec4(1.0, 0.0, 0.0, 1.0);
}
