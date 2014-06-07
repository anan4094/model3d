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

float getPowInOut(const float fpow,float t) {
	if ((t*=2)<1) return 0.5*pow(t,fpow);
	return 1-0.5*abs(pow(2-t,fpow));
}

float ease::linear(float r){
	return r;
}

float ease::quadInOut(float r){
	return getPowInOut(2,r);
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
Animation::Animation():func(ease::quadInOut){
    Animation::sm_nAnims.push_back(this);
}

void Animation::update(){
	if (m_bIsRunning) {
		long interval=Stage::sm_iCurrentTime-m_iStartTime;
		float rate = ((float)interval/m_iDelay);
		if (rate>1) {
			return;
		}
		rate=func(rate);
		updateByWeight(rate);
	}
}

void Animation::updateByWeight(float r){

}


