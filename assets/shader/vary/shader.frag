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

uniform sampler2D tex;
varying vec3 texCoordVarying;
varying vec3 enor;
varying vec3 epos;

const int maxLightCount = 32;
uniform lightParams light[maxLightCount];

float shininess = 32;

vec4 specular = vec4(.35,.35,.35,1);

void DirectionalLight(int i, vec3 eye, vec3 epos, vec3 normal,
                      inout vec4 amb, inout vec4 diff, inout vec4 spec){
    float dotVP = max(0, dot(normal, normalize(vec3(light[i].position))));
    float dotHV = max(0, dot(normal, normalize(eye+normalize(vec3(light[i].position)))));
    
    amb += light[i].ambient;
    diff += light[i].diffuse * dotVP;
    spec += light[i].specular * pow(dotHV, shininess);
}

void PointLight(int i, vec3 eye, vec3 epos, vec3 normal, 
                inout vec4 amb, inout vec4 diff, inout vec4 spec){
    vec3 VP = vec3(light[i].position) - epos;
    float d = length(VP);
    VP = normalize(VP);
    
    float att = 1.0/(light[i].constantAttenuation + light[i].linearAttenuation*d + light[i].quadraticAttenuation*d*d);
    vec3 h = normalize(VP+eye);
    //vec3 h = normalize(reflect(VP,normal));
    
    float dotVP = max(0, dot(normal, VP));
    float dotHV = max(0, dot(normal, h));
    //float dotHV = max(0, dot(eye,h));
    
    amb += light[i].ambient * att;
    diff += light[i].diffuse * dotVP * att;
    spec += light[i].specular * pow(dotHV,shininess) * att;
}

void main (void)
{
	vec4 amb = vec4(0);
    vec4 diff = vec4(0);
    vec4 spec = vec4(0);

    vec3 eye = -normalize(epos);

    for (int i=0; i<maxLightCount; i++){
        if (light[i].enabled == false){
            continue;
		}
        if (light[i].position.w == 0){
            DirectionalLight(i, eye, epos, enor, amb, diff, spec);
        }else if (light[i].spotCutoff == 180.0){
            PointLight(i, eye, epos, enor, amb, diff, spec);
        }else{
            //SpotLight(i, eye, epos, normal, amb, diff, spec);
        }
    }
    
	vec4 sceneColor = texture2D(tex,texCoordVarying.st);

    // write Total Color:
    gl_FragColor = sceneColor*(amb + diff)+ spec;
}