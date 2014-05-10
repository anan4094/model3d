//
//  Light.cpp
//  model3d
//
//  Created by 魏裕群 on 14-5-10.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "Light.h"

void Light::directionalLight(float x,float y,float z){
    position[0]=x;
    position[1]=y;
    position[2]=z;
    position[3]=1;
}

void Light::pointLight(float x,float y,float z){
    position[0]=x;
    position[1]=y;
    position[2]=z;
    position[3]=0;
    spotCutoff=180.0f;
}

void Light::spotLight(float x,float y,float z){
}