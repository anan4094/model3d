//
//  Application.h
//  model3d
//
//  Created by 魏裕群 on 14-4-17.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__Application_mac__
#define __model3d__Application_mac__

class Application{
public:
    Application();
    virtual ~Application();
    int run();
    static Application* sharedInstance();
protected:
    long m_lAnimationInterval;  //micro second
    static Application * sm_pSharedApplication;
};
#endif /* defined(__model3d__Application_mac__) */
