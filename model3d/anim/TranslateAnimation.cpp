//
//  TranslateAnimation.cpp
//  model3d
//
//  Created by 魏裕群 on 14-5-23.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "TranslateAnimation.h"
#include "Stage.h"

TranslateAnimation::TranslateAnimation(float x,float y,float z,long delay){
    m_fTarX = x;
    m_fTarY = y;
    m_fTarZ = z;
    m_iDelay = delay;
}

void TranslateAnimation::updateByWeight(float rate){
    m_nNode->x=m_fOriX+(m_fTarX-m_fOriX)*rate;
    m_nNode->y=m_fOriY+(m_fTarY-m_fOriY)*rate;
    m_nNode->z=m_fOriZ+(m_fTarZ-m_fOriZ)*rate;
}

Animation* TranslateAnimation::start(){
    m_fOriX = m_nNode->x;
    m_fOriY = m_nNode->y;
    m_fOriZ = m_nNode->z;
    return Animation::start();
}
