#version 120

struct lightParams{
    bool enabled;
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 spotDirection;
    float spotCutoff;
    float spotExponent;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

varying vec3 N;
varying vec3 v;
varying vec3 texCoordVarying;

attribute vec4 position;
attribute vec4 normal;
attribute vec3 texcoord;


const int maxLightCount = 32;
uniform lightParams light[maxLightCount];
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;

void DirectionalLight(int i, vec3 eye, vec3 epos, vec3 normal,
                      inout vec4 amb, inout vec4 diff, inout vec4 spec)
{
    float dotVP = max(0, dot(normal, normalize(vec3(light[i].position))));
    float dotHV = max(0, dot(normal, normalize(eye+normalize(vec3(light[i].position)))));
    
    amb += light[i].ambient;
    diff += light[i].diffuse * dotVP;
    spec += light[i].specular * pow(dotHV, gl_FrontMaterial.shininess);
}

void main(void)
{
    vec4 pos4 = modelViewMatrix * position;
    v = vec3(pos4.xyz);
    vec4 nor = normalMatrix*normal;
    N = normalize(nor.xyz);
    texCoordVarying = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}