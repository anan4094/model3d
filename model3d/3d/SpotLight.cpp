//
//  SpotLight.cpp
//  model3d
//
//  Created by 魏裕群 on 14-5-18.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "SpotLight.h"
#include <stdio.h>

SpotLight::SpotLight(GLfloat cutoff):Light(){
	position[3]=1;
	if (cutoff>90.0f){
		cutoff=90.0f;
	}else if (cutoff<0){
		cutoff=0;
	}
	spotCutoff=cutoff;
}

void SpotLight::setPosition(GLfloat x,GLfloat y,GLfloat z){
	position[0]=x;
	position[1]=y;
	position[2]=z;
}
void SpotLight::setDirection(GLfloat x,GLfloat y,GLfloat z){
	spotDirection[0]=x;
	spotDirection[1]=y;
	spotDirection[2]=z;
}
