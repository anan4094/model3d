#version 120

attribute vec4 position;
attribute vec4 normal;
attribute vec2 texcoord;

varying float nDotVP;
varying vec2 texCoordVarying;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;
uniform vec4 diffuseColor;

void main()
{    
    vec4 nor = normalMatrix*normal;
    vec3 eyeNormal = normalize(nor.xyz);
    vec3 lightPosition = vec3(0.0, 0.0, 1.0);
    
    nDotVP = max(0.2, dot(eyeNormal, normalize(lightPosition)));
	texCoordVarying = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}
