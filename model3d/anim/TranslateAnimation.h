//
//  TranslateAnimation.h
//  model3d
//
//  Created by 魏裕群 on 14-5-23.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__TranslateAnimation__
#define __model3d__TranslateAnimation__
#include "BaseAnimation.h"

class TranslateAnimation:public Animation{
private:
    float m_fStartTime;
    float m_fOriX;
    float m_fOriY;
    float m_fOriZ;
    float m_fTarX;
    float m_fTarY;
    float m_fTarZ;
public:
    virtual void update();
    virtual Animation* start();
    TranslateAnimation(float x,float y,float z,long delay);
};

#endif /* defined(__model3d__TranslateAnimation__) */
