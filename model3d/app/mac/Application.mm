//
//  Application.cpp
//  model3d
//
//  Created by 魏裕群 on 14-4-17.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#import "Application.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>

static long getCurrentMillSecond()
{
    long lLastTime = 0;
    struct timeval stCurrentTime;
    
    gettimeofday(&stCurrentTime,NULL);
    lLastTime = stCurrentTime.tv_sec*1000+stCurrentTime.tv_usec*0.001; //millseconds
    return lLastTime;
}

Application* Application::sm_pSharedApplication = 0;

Application::Application():m_lAnimationInterval(1.0f/60.0f*1000.0f){
    if (!sm_pSharedApplication) {
        return;
    }
    sm_pSharedApplication = this;
}

Application::~Application(){
    if (this!=sm_pSharedApplication) {
        return;
    }
    sm_pSharedApplication = 0;
}

int Application::run(){
    long lastTime = 0L;
    long curTime = 0L;
    
    while (1)
    {
        lastTime = getCurrentMillSecond();
        
        curTime = getCurrentMillSecond();
        if (curTime - lastTime < m_lAnimationInterval)
        {
            usleep(static_cast<useconds_t>((m_lAnimationInterval - curTime + lastTime)*1000));
        }
    }
    return 0;
}
