#version 120

varying vec3 enor;
varying vec3 epos;
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
    epos = vec3(pos4)/pos4.w;
    enor = normalize(vec3(normalMatrix*normal));
    texCoordVarying = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}