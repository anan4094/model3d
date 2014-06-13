//
//  Animation.h
//  model3d
//
//  Created by 魏裕群 on 14-5-23.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__Animation__
#define __model3d__Animation__

#include "Drawable.h"
#include <functional>

typedef float (*easeFunc)(float);

namespace ease{
	float linear(float r);
	float quadInOut(float r);
    float quadIn(float r);
    float quadOut(float r);
    float cubicInOut(float r);
    float cubicIn(float r);
    float cubicOut(float r);
    float quartInOut(float r);
    float quartIn(float r);
    float quartOut(float r);
    float quintInOut(float r);
    float quintIn(float r);
    float quintOut(float r);
    float sineIn(float r);
    float sineOut(float r);
    float sineInOut(float r);
}


class Animation{
protected:
    long m_nStartTime;
    long m_nDuration;
    Drawable *m_iNode;
    int m_nStatus;

	easeFunc m_pfnEase;
    std::function< void()> m_pfnStart;
    std::function< void()> m_pfnComplete;
    std::function< void(float r)> m_pfnUpdate;
public:
    static Animation* add(Drawable *node,Animation *anim);
    static vector<Animation*>sm_gAnims;
    
    virtual Animation* start();
    void update();
	virtual void updateByWeight(float r);
	template<typename T>
	Animation* setStartFunction(T *listener,void (T::*pfn)(Drawable *node)){
		m_fnStart = std::bind(pfn, listener, m_iNode);
		return this;
	}
	template<typename T>
	Animation* setCompleteFunction(T *listener,void (T::*pfn)(Drawable *node)){
		m_pfnComplete = std::bind(pfn, listener, m_iNode);
		return this;
	}
	template<typename T>
	Animation* setUpdateFunction(T *listener,void (T::*pfn)(Drawable *node,float r)){
		m_pfnUpdate = std::bind(pfn, listener, m_iNode, std::placeholders::_1);
		return this;
	}

	Animation* setUpdateFunction(std::function<void (float)>);
    Animation();
};

#endif /* defined(__model3d__Animation__) */
