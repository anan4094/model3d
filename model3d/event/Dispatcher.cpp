//
//  Dispatcher.cpp
//  model3d
//
//  Created by 魏裕群 on 14-4-16.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "Dispatcher.h"
#include "Stage.h"
#include "Node.h"
Dispatcher::Dispatcher(){

}
bool Dispatcher::dispatcherTouchEvent(MotionEvent &event){
	Node *_list[10];
	int count = 0;
	Node *node=Stage::sharedInstance()->runningScene();
	if (!node){
		return false;
	}
	bool ret=false;
	do {
		_list[count++]=node;
		ret = node->dispatcherTouchEvent(event);
		node=event.target;
	} while (ret);
	while (count--){
		ret = _list[count]->onTouchEvent(event);
		if (!ret){
			break;
		}
	}
	return true;
}

bool Dispatcher::dispatcherKeyEvent(KeyEvent &event){
	Node *_list[10];
	int count = 0;
	Node *node=Stage::sharedInstance()->runningScene();
	if (!node){
		return false;
	}
	bool ret=false;
	do {
		_list[count++]=node;
		ret = node->dispatcherKeyEvent(event);
		node=event.target;
	} while (ret);
	while (count--){
		ret = _list[count]->onKeyEvent(event);
		if (!ret){
			break;
		}
	}
	return true;
}
Dispatcher* Dispatcher::sm_pSharedDispatcher = nullptr;
Dispatcher* Dispatcher::sharedInstance(){
	if (!sm_pSharedDispatcher){
		sm_pSharedDispatcher = new Dispatcher();
	}
	return sm_pSharedDispatcher;
}
