#version 120
uniform sampler2D tex;

varying float nDotVP;
varying vec2 texCoordVarying;

void main()
{
    gl_FragColor = texture2D(tex,texCoordVarying.st) * nDotVP;
}
