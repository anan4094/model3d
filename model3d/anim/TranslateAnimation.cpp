//
//  TranslateAnimation.cpp
//  model3d
//
//  Created by 魏裕群 on 14-5-23.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "TranslateAnimation.h"
#include "Stage.h"

TranslateAnimation::TranslateAnimation(float x,float y,float z,long duration){
    m_fTarX = x;
    m_fTarY = y;
    m_fTarZ = z;
    m_nDuration = duration;
}

void TranslateAnimation::updateByWeight(float rate){
    m_iNode->x=m_fOriX+(m_fTarX-m_fOriX)*rate;
    m_iNode->y=m_fOriY+(m_fTarY-m_fOriY)*rate;
    m_iNode->z=m_fOriZ+(m_fTarZ-m_fOriZ)*rate;
}

Animation* TranslateAnimation::start(){
    m_fOriX = m_iNode->x;
    m_fOriY = m_iNode->y;
    m_fOriZ = m_iNode->z;
    return Animation::start();
}
