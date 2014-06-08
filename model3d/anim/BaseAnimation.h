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

typedef void (Object::*animStart)(Drawable* node,void* data);
typedef void (Object::*animComplete)(Drawable* node,void* data);
typedef void (Object::*animUpdate)(Drawable* node,float r,void* data);


class Animation{
protected:
    long m_iStartTime;
    long m_iDuration;
    Drawable *m_nNode;
    bool m_bIsRunning;
	easeFunc func;
    Object *m_nListener;
    animStart m_fStart;
    animComplete m_fComplete;
    animUpdate m_fUpdate;
public:
    static Animation* add(Drawable *node,Animation *anim);
    static vector<Animation*>sm_nAnims;
    
    virtual Animation* start();
    void update();
	virtual void updateByWeight(float r);
    Animation* setListener(Object *listener){m_nListener=listener;return this;};
    Animation* setStartFunction(animStart func){m_fStart=func;return this;};
    Animation* setCompleteFunction(animComplete func){m_fComplete=func;return this;};
    Animation* setUpdateFunction(animUpdate func){m_fUpdate=func;return this;};
    Animation();
};

#endif /* defined(__model3d__Animation__) */
