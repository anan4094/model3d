//
//  Animation.cpp
//  model3d
//
//  Created by 魏裕群 on 14-5-23.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "BaseAnimation.h"
#include <math.h>
#include "Stage.h"

float getPowIn(const float fpow,float t){
    return pow(t, fpow);
}

float getPowOut(const float fpow,float t){
    return 1- pow(1-t,fpow);
}

float getPowInOut(const float fpow,float t) {
	if ((t*=2)<1) return 0.5*pow(t,fpow);
	return 1-0.5*abs(pow(2-t,fpow));
}

float ease::linear(float r){
	return r;
}

float ease::quadIn(float r){
	return getPowIn(2,r);
}

float ease::quadOut(float r){
	return getPowInOut(2,r);
}

float ease::quadInOut(float r){
	return getPowInOut(2,r);
}

float ease::cubicIn(float r){
	return getPowIn(3,r);
}

float ease::cubicOut(float r){
	return getPowInOut(3,r);
}

float ease::cubicInOut(float r){
	return getPowInOut(3,r);
}

float ease::quartInOut(float r){
	return getPowInOut(4,r);
}

float ease::quartIn(float r){
	return getPowInOut(4,r);
}

float ease::quartOut(float r){
	return getPowInOut(4,r);
}

float ease::quintIn(float r){
	return getPowInOut(5,r);
}

float ease::quintOut(float r){
	return getPowInOut(5,r);
}

float ease::quintInOut(float r){
	return getPowInOut(5,r);
}

float ease::sineIn(float r){
    return 1-cos(r*M_PI/2);
}

/**
 * @method sineOut
 * @static
 **/
float ease::sineOut(float r){
    return sin(r*M_PI/2);
}

/**
 * @method sineInOut
 * @static
 **/
float ease::sineInOut(float r){
    return -0.5*(cos(M_PI*r) - 1);
}

vector<Animation*> Animation::sm_gAnims = vector<Animation*>();

Animation* Animation::add(Drawable *node,Animation *anim){
    anim->m_iNode=node;
    return anim;
}

Animation* Animation::start(){
    m_nStatus=1;
    return this;
}
Animation::Animation():m_pfnEase(ease::quadInOut),m_pfnStart(nullptr),m_pfnUpdate(nullptr),m_pfnComplete(nullptr),m_nStatus(0){
    Animation::sm_gAnims.push_back(this);
}

void Animation::update(){
	if (m_nStatus==1){
		m_nStartTime=Stage::sm_iCurrentTime;
		if (!m_pfnUpdate._Empty()) {
			m_pfnUpdate(0);
		}
		if (!m_pfnStart._Empty()){
			m_pfnStart();
		}
		m_nStatus=2;
	}else if (m_nStatus==2) {
		long interval=Stage::sm_iCurrentTime-m_nStartTime;
		float rate = ((float)interval/m_nDuration);
		if (rate>1) {
			if (!m_pfnUpdate._Empty()) {
				m_pfnUpdate(1);
			}
            if (!m_pfnComplete._Empty()) {
                m_pfnComplete();
            }
            vector<Animation*>::iterator itr = Animation::sm_gAnims.begin(),end=Animation::sm_gAnims.end();
            while (itr != end){
                if (*itr == this)Animation::sm_gAnims.erase(itr);
                break;
                ++itr; 
            }
			return;
		}
        rate=m_pfnEase(rate);
        if (!m_pfnUpdate._Empty()) {
            m_pfnUpdate(rate);
        }
		updateByWeight(rate);
	}
}

void Animation::updateByWeight(float r){
}
Animation* Animation::setStartFunction(std::function<void ()> func){
	m_pfnStart=func;
	return this;
}

Animation* Animation::setUpdateFunction(std::function<void (float r)>func){
	m_pfnUpdate=func;
	return this;
}

