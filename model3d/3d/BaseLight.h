//
//  BaseLight.h
//  model3d
//
//  Created by 魏裕群 on 14-5-18.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef model3d_BaseLight_h
#define model3d_BaseLight_h

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
    
    Light();
    
    virtual void setPosition(GLfloat x,GLfloat y,GLfloat z)=0;
    virtual void setDirection(GLfloat x,GLfloat y,GLfloat z)=0;
    void setAmbient(GLfloat r,GLfloat g,GLfloat b);
    void setAmbient(const char *color);
    
    //    void directionalLight(float x,float y,float z);
    //    void pointLight(float x,float y,float z);
    //    void spotLight(float x,float y,float z);
};

#endif
