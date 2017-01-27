
// INPUTS du Fragment Shader = OUTPUTS du Vertex Shader

varying vec2 v_TexCoords;

uniform float u_Time;

uniform sampler2D u_Texture;

const vec3 luma = vec3(0.30, 0.59, 0.11);

void main(void)
{
    vec2 texCoords = v_TexCoords;

    // equation d'une onde = Amplitude * cos(2*Pi*Frequence*Temps + Phase);
    float amplitude = 1.0 / 100.0;
    float frequence = v_TexCoords.y;
    texCoords.x += amplitude * cos(frequence * u_Time * 2.0*3.14159);
    
    vec4 texColor = texture2D(u_Texture, texCoords);
    // variable predefinies OBLIGATOIRE en OUTPUT
    // vec4 gl_FragColor
    gl_FragColor = vec4(vec3(dot(luma, texColor.rgb)), 1.0);   
}
