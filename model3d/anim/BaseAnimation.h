//
//  Animation.h
//  model3d
//
//  Created by 魏裕群 on 14-5-23.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__Animation__
#define __model3d__Animation__

#include "Node.h"

class Animation{
private:
    long m_nStartTime;
public:
    static Animation* add(Node *node,Animation *anim);
    virtual Animation* start();
    virtual void update()=0;
};

#endif /* defined(__model3d__Animation__) */
