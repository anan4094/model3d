#version 120

varying vec3 N;
varying vec3 v;
varying vec3 texCoordVarying;

attribute vec4 position;
attribute vec4 normal;
attribute vec3 texcoord;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;
void main(void)
{
    vec4 pos4 = modelViewMatrix * position;
    v = vec3(pos4.xyz);
    vec4 nor = normalMatrix*normal;
    N = normalize(nor.xyz);
    texCoordVarying = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}