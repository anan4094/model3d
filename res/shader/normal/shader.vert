#version 120

attribute vec4 position;
attribute vec4 normal;

varying vec4 colorVarying;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;
uniform vec4 diffuseColor;

void main()
{    
    vec4 nor = normalMatrix*normal;
    vec3 eyeNormal = normalize(nor.xyz);
    vec3 lightPosition = vec3(0.0, 0.0, -1.0);
    
    float nDotVP = max(0.0, dot(eyeNormal, normalize(lightPosition)));
    
    colorVarying = diffuseColor * nDotVP;
    
    gl_Position = modelViewProjectionMatrix * position;
}
