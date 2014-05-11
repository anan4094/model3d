//
//  Light.h
//  model3d
//
//  Created by 魏裕群 on 14-5-10.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__Light__
#define __model3d__Light__

#include "platform.h"

class Light{
public:
	bool enabled;
    GLfloat position[4];
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat spotDirection[3];
    GLfloat spotCutoff;
    GLfloat spotExponent;
    GLfloat constantAttenuation;
    GLfloat linearAttenuation;
    GLfloat quadraticAttenuation;
    
    void directionalLight(float x,float y,float z);
    void pointLight(float x,float y,float z);
    void spotLight(float x,float y,float z);
};

#endif /* defined(__model3d__Light__) */
