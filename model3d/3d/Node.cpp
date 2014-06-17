//
//  CNode.cpp
//  model3d
//
//  Created by 魏裕群 on 14-4-11.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//
#include "Node.h"

Node::Node():rax(0),ray(0),raz(1),radian(0),x(0),y(0),z(0),scalex(1),scaley(1),scalez(1){
}

void Node::addSubNode(Node *node){
    m_iSubNodes.push_back(node);
}

bool Node::dispatcherTouchEvent(MotionEvent &event){
	return false;
}

bool Node::onTouchEvent(MotionEvent &event){
	return false;
}

bool Node::dispatcherKeyEvent(KeyEvent &event){
	return false;
}

bool Node::onKeyEvent(KeyEvent &event){
	return false;
}
