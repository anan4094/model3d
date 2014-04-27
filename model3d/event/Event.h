//
//  Event.h
//  model3d
//
//  Created by 魏裕群 on 14-4-21.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef model3d_Event_h
#define model3d_Event_h

class Node;

typedef enum _MotionAction{
    MotionDown
    ,MotionMove
    ,MotionCancel
	,MotionUp
}MotionAction;

typedef struct _MotionEvent{
	MotionAction action;
	double x;
	double y;
	void *userData;
	Node *target;
}MotionEvent ;

#endif
