#version 120
uniform sampler2D tex;
varying vec3 texCoordVarying;
varying vec3 N;
varying vec3 v;

void main (void)
{
	vec3 lightPosition = vec3(30,30,0);
    vec3 L = normalize(lightPosition - v);
    vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)
    vec3 R = normalize(-reflect(L,N));
	vec4 specular = vec4(.35,.35,.35,1);
	float shininess = 32;
    
	vec4 sceneColor = texture2D(tex,texCoordVarying.st);
    //calculate Ambient Term:
    float Iamb = 0.1f;
    
    //calculate Diffuse Term:
    float Idiff = max(dot(N,L), 0.0);
    
    // calculate Specular Term:
    vec4 Ispec = specular*pow(max(dot(R,E),0.0),shininess);
    // write Total Color:
    gl_FragColor = sceneColor*(Iamb + Idiff)+Ispec;
}