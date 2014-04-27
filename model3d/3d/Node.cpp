//
//  CNode.cpp
//  model3d
//
//  Created by 魏裕群 on 14-4-11.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//
#include "Node.h"

void Node::addSubNode(Node *node){
    m_iSubNodes.push_back(node);
}

bool Node::dispatcherTouchEvent(MotionEvent &event){
	return false;
}

bool Node::onTouchEvent(MotionEvent &event){
	return false;
}
