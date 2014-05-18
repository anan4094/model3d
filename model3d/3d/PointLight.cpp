//
//  PointLight.cpp
//  model3d
//
//  Created by 魏裕群 on 14-5-18.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "PointLight.h"
#include <stdio.h>

PointLight::PointLight():Light(){
    position[3]=1;
    spotCutoff=180;
}

PointLight::PointLight(GLfloat x,GLfloat y,GLfloat z):Light(){
    position[3]=1;
    spotCutoff=180;
    setPosition(x, y, z);
}

void PointLight::setPosition(GLfloat x,GLfloat y,GLfloat z){
    position[0]=x;
    position[1]=y;
    position[2]=z;
}
void PointLight::setDirection(GLfloat x,GLfloat y,GLfloat z){
    printf("point light can't set direction\n");
}
