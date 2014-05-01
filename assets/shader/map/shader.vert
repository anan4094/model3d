#version 120

attribute vec4 position;
attribute vec2 texcoord;

varying vec2 texCoordVarying;

uniform mat4 modelViewProjectionMatrix;

void main(){    
	texCoordVarying = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}
