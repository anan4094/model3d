//
//  DirectionalLight.h
//  model3d
//
//  Created by 魏裕群 on 14-5-18.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__DirectionalLight__
#define __model3d__DirectionalLight__

#include "BaseLight.h"

class DirectionalLight:public Light{
public:
    DirectionalLight();
    DirectionalLight(GLfloat x,GLfloat y,GLfloat z);
    virtual void setPosition(GLfloat x,GLfloat y,GLfloat z);
    virtual void setDirection(GLfloat x,GLfloat y,GLfloat z);
};

#endif /* defined(__model3d__DirectionalLight__) */
