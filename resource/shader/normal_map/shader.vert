#version 120

attribute vec4 position;
attribute vec4 normal;
attribute vec3 texcoord;

varying float nDotVP;
varying vec3 texCoordVarying;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;
uniform vec4 diffuseColor;

void main()
{    
    vec4 nor = normalMatrix*normal;
    vec3 eyeNormal = normalize(nor.xyz);
    vec3 lightPosition = vec3(0.0, 0.0, 1.0);
    
    nDotVP = max(0.0, dot(eyeNormal, normalize(lightPosition)));
    nDotVP = min(nDotVP+0.2,1.0);
	texCoordVarying = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}
