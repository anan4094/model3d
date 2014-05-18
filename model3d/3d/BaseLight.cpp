//
//  Light.cpp
//  model3d
//
//  Created by 魏裕群 on 14-5-10.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "BaseLight.h"

Light::Light():spotCutoff(0),spotExponent(0),constantAttenuation(1),linearAttenuation(0),quadraticAttenuation(0),enabled(true){
    position[0]=0;
    position[1]=0;
    position[2]=0;
    position[3]=0;
    ambient[0]=.618f;
    ambient[1]=.618f;
    ambient[2]=.618f;
    ambient[3]=1;
    diffuse[0]=1;
    diffuse[1]=1;
    diffuse[2]=1;
    diffuse[3]=1;
    specular[0]=1;
    specular[1]=1;
    specular[2]=1;
    specular[3]=1;
    spotDirection[0]=0;
    spotDirection[1]=0;
    spotDirection[2]=1;
}

void Light::setAmbient(GLfloat r, GLfloat g, GLfloat b){
}

void Light::setAmbient(const char *color){
}