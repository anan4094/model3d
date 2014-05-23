//
//  Animation.cpp
//  model3d
//
//  Created by 魏裕群 on 14-5-23.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "BaseAnimation.h"
#include "Stage.h"

Animation* Animation::add(Node *node,Animation *anim){
    return anim;
}

Animation* Animation::start(){
    m_nStartTime=Stage::sm_nCurrentTime;
    return this;
}


