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

vector<Animation*> Animation::sm_nAnims = vector<Animation*>();

Animation* Animation::add(Drawable *node,Animation *anim){
    anim->m_nNode=node;
    return anim;
}

Animation* Animation::start(){
    m_iStartTime=Stage::sm_iCurrentTime;
    m_bIsRunning=true;
    return this;
}
Animation::Animation():func(ease::quadInOut),m_nListener(nullptr),m_fStart(nullptr),m_fUpdate(nullptr),m_fComplete(nullptr){
    Animation::sm_nAnims.push_back(this);
}

void Animation::update(){
	if (m_bIsRunning) {
		long interval=Stage::sm_iCurrentTime-m_iStartTime;
		float rate = ((float)interval/m_iDuration);
		if (rate>1) {
            if (m_nListener&&m_fComplete) {
                (m_nListener->*m_fComplete)(m_nNode, nullptr);
            }
            vector<Animation*>::iterator itr = Animation::sm_nAnims.begin(),end=Animation::sm_nAnims.end();
            while (itr != end){
                if (*itr == this)Animation::sm_nAnims.erase(itr);
                break;
                ++itr; 
            }
			return;
		}
        rate=func(rate);
        if (rate==0&&m_nListener&&m_fStart){
            (m_nListener->*m_fStart)(m_nNode, nullptr);
        }
        if (m_nListener&&m_fUpdate) {
            (m_nListener->*m_fUpdate)(m_nNode, rate, nullptr);
        }
		updateByWeight(rate);
	}
}

void Animation::updateByWeight(float r){

}


