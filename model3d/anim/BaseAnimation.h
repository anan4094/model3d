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

class Animation{
protected:
    long m_iStartTime;
    long m_iDelay;
    Drawable *m_nNode;
    bool m_bIsRunning;
public:
    static Animation* add(Drawable *node,Animation *anim);
    static vector<Animation*>sm_nAnims;
    
    virtual Animation* start();
    virtual void update()=0;
    Animation();
};

#endif /* defined(__model3d__Animation__) */
