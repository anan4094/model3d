//
//  CNode.h
//  model3d
//
//  Created by 魏裕群 on 14-4-11.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__CNode__
#define __model3d__CNode__

#include "CScene.h"

class CNode{
protected:
    CScene    *m_piScene;
public:
    inline void setScene(CScene*scene){m_piScene = scene;}
    virtual void draw() = 0;
};

#endif /* defined(__model3d__CNode__) */
