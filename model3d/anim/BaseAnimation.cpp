//
//  Animation.cpp
//  model3d
//
//  Created by 魏裕群 on 14-5-23.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "BaseAnimation.h"
#include "Stage.h"

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
Animation::Animation(){
    Animation::sm_nAnims.push_back(this);
}


