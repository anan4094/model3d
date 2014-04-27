//
//  Dispatcher.h
//  model3d
//
//  Created by 魏裕群 on 14-4-16.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__Dispatcher__
#define __model3d__Dispatcher__
#include "Event.h"

class Dispatcher{
private:
	static Dispatcher* sm_pSharedDispatcher;
	Dispatcher();
public:
	bool dispatcherTouchEvent(MotionEvent &event);
	static Dispatcher* sharedInstance();
};

#endif /* defined(__model3d__Dispatcher__) */
