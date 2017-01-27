#version 330

// INPUTS du Fragment Shader = OUTPUTS du Vertex Shader

in FragmentIn
{
    vec4 v_Position;
    vec2 v_TexCoords;
    vec3 v_Normal;
    mat4 modelMatrix;
} IN;

in vec4 v_Position;
in vec2 v_TexCoords;
in vec3 v_Normal;

in mat4 modelMatrix;

uniform sampler2D u_texture;
uniform float u_power;
uniform vec3 u_light_pos;
uniform vec3 u_cam_pos;

struct Attenuation
{
    float constante;
    float linear;
    float quadratic;
};

struct Light
{
    vec3 position;
    vec3 colorD;
    vec3 colorS;
    vec3 colorA;
    vec3 skyColor;
    vec3 groundColor;
    float power;
    float shininess;
    float ambient_factor;
    Attenuation attenuation;
} light;

out vec4 OutColor;

void main(void)
{
    //light initialization
    light.position = vec3(0.0, -1.0, 0.0);
    light.colorD = vec3(1.0, 1.0, 1.0);
    light.colorS = vec3(1.0, 1.0, 1.0);
    light.colorA = vec3(1.0, 1.0, 1.0);
    light.skyColor = vec3(0.0, 0.0, 1.0);
    light.groundColor = vec3(0.0, 1.0, 0.0);
    light.power = u_power;
    light.shininess = 85.0;
    light.ambient_factor = 0.1;
    light.attenuation.constante = 1.0;
    light.attenuation.linear = 0.0;
    light.attenuation.quadratic = 0.4;

    //texture
    vec2 texCoords = vec2(v_TexCoords.x, 1.0 - v_TexCoords.y);
    vec4 texColor = texture2D(u_texture, texCoords);

    //attenuation
    float constante_factor = light.attenuation.constante;
    float linear_factor = light.attenuation.linear * distance(v_Position.xyz, light.position);
    float quadratic_factor = light.attenuation.quadratic * pow(distance(v_Position.xyz, light.position), 2);
    float attenuation_factor = 1.0 / (constante_factor + linear_factor + quadratic_factor);

    //diffuse
    vec3 normal = normalize(v_Normal);

    vec3 L = normalize(light.position - v_Position.xyz);

    float theta = dot(normal, L);
    float I = max(cos(theta), 0);

    vec3 diffuse_color = I * light.colorD;

    //specular
    mat3 rotMat = mat3(
        modelMatrix[0][0], modelMatrix[0][1], modelMatrix[0][2],
        modelMatrix[1][0], modelMatrix[1][1], modelMatrix[1][2],
        modelMatrix[2][0], modelMatrix[2][1], modelMatrix[2][2]);

    vec3 d = modelMatrix[3].xyz;
    vec3 cam_pos = -d * rotMat;

    vec3 V = normalize(cam_pos - v_Position.xyz);
    vec3 R = reflect(L, normal);

    float angle = dot(normalize(V), R);

    vec3 specular_color = light.colorS * float(gl_FrontFacing) * pow(max(angle, 0), light.shininess);

    //hemispheric
    //init sky direction
    vec3 S = vec3(0.0, 1.0, 0.0);

    float factor = clamp(dot(normal, S), -1, 1);

    vec3 hemispheric_color = mix(light.groundColor,light.skyColor, factor);

    //ambient
    vec3 ambient_color = light.colorA * hemispheric_color * light.ambient_factor;

    //final
    vec3 light_color = ambient_color + (diffuse_color + specular_color) * attenuation_factor;

    vec3 final_color = light_color * light.power * texColor.rgb;

    OutColor = vec4(final_color, texColor.a * 1.0);
}
