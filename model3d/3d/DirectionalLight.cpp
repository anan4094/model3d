//
//  DirectionalLight.cpp
//  model3d
//
//  Created by 魏裕群 on 14-5-18.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "DirectionalLight.h"
#include <stdio.h>

DirectionalLight::DirectionalLight():Light(){
    position[3]=0;
}

DirectionalLight::DirectionalLight(GLfloat x,GLfloat y,GLfloat z):Light(){
    position[3]=0;
    setDirection(x, y, z);
}

void DirectionalLight::setPosition(GLfloat x,GLfloat y,GLfloat z){
    printf("Directional light can't set position\n");

}
void DirectionalLight::setDirection(GLfloat x,GLfloat y,GLfloat z){
    position[0]=x;
    position[1]=y;
    position[2]=z;
}
