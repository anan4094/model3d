//
//  CNode.h
//  model3d
//
//  Created by 魏裕群 on 14-4-11.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__Node__
#define __model3d__Node__
#include "Event.h"
#include <vector>
using namespace std;

class Scene;

class Node{
protected:
    vector<Node*> m_iSubNodes;
public:
    void addSubNode(Node* node);
    virtual void draw() = 0;
    virtual bool screenSizeChange(int width,int height)=0;
    bool dispatcherTouchEvent(MotionEvent &event);
	bool dispatcherKeyEvent(KeyEvent &event);
	virtual bool onTouchEvent(MotionEvent &event);
	virtual bool onKeyEvent(KeyEvent &event);
};

#endif /* defined(__model3d__Node__) */
