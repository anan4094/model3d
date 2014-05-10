//
//  Scene.cpp
//  model3d
//
//  Created by 魏裕群 on 14-4-11.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "Scene.h"
#include "Stage.h"
#include <vector>
using namespace std;



void Scene::draw(){
    vector<Node*>::iterator itr = m_iSubNodes.begin(),end = m_iSubNodes.end();
    for (; itr!=end; itr++) {
        (*itr)->draw();
    }
}

bool Scene::screenSizeChange(int width, int height){
    reshape(width, height);
    return true;
}

void Scene::reshape(int w, int h){
	glViewport(0,0,w,h);
    Stage::sharedInstance()->currentCamera()->frustum(-1.0f*w/h, 1.0f*w/h, -1.0f, 1.0f,2.0f, 40.0f);
}
