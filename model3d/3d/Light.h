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
    float position[4];
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float spotDirection[3];
    float spotCutoff;
    float spotExponent;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    
    void directionalLight(float x,float y,float z);
    void pointLight(float x,float y,float z);
    void spotLight(float x,float y,float z);
};

#endif /* defined(__model3d__Light__) */
